#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define square(x) ((x)*(x))
#define maxpop   200  /*Max population */
#define maxvar    700  /*Max no. of variables*/
#define maxfun    2  /*Max no. of functions */
#define maxfeature 40 /*Max no. of features*/
#define maxdatapoints 10000 /*Max no. of data points*/
#define nfunc 2     /*Number of objective functions*/

int lim_r[maxvar][2];/*Limits of variable in array*/
double seed,      /*Random Seed*/
pcross = 1;        /*Cross-over Probability*/
double di = 20,        /*Distribution Index for the Cross-over*/
dim = 20,           /*Distribution Index for the Mutation*/
delta_fit,     /* variables required for fitness for fitness sharing */
min_fit,
front_ratio;
int optype,      /*Cross-over type*/
sharespace;    /*Sharing space (either parameter or fitness)*/
double coef[maxvar]; /*Variable used for decoding*/
static int popsize = 100,  /*Population Size*/
chrom,    /*Chromosome size*/
gener = 100, /*No of generations*/
nvar = 98,
instancenum = 1555,
numF = 2;
static char
data_name[20] = "2d-5c-1_1_23";

typedef struct       /*individual properties*/
{
	int rank,              /*Rank of the individual*/
		flag;              /*Flag for ranking*/
	int xreal[maxvar], /*list of real variables*/
		mut[2];
	double fitness[maxfun],/*Fitness values */
		cub_len,             /*crowding distance of the individual*/
		error;              /* overall constraint violation for the individual*/
}individual;        /*Structure defining individual*/

typedef struct
{
	int maxrank;            /*Maximum rank present in the population*/
	double rankrat[maxpop];  /*Rank Ratio*/
	int rankno[maxpop];     /*Individual at different ranks*/
	individual ind[maxpop], /*Different Individuals*/
		* ind_ptr;
}population;             /*Population Structure*/

/* Data, Neighborhood Data*/
double Data[maxdatapoints][maxfeature], Distances[maxvar][maxvar], Centroids[maxvar][maxfeature], Compactness[maxvar];
int closures[maxvar][maxdatapoints], closure_size[maxvar], closure_neighborhood[maxvar][maxvar];

#include "mrand.h"              /*Random number generator*/
#include "clusterassign.h"      /*Output of cluster assignments*/
/*#include "input.h"              /*File takes input from user*/
#include "decode_cluster_assignment.h"  /*Decode the linked based representation to cluster assignments*/
#include "init.h"               /*Random initialization of the population*/
#include "ranking.h"            /*File creating the Pareto Fronts*/
#include "func-con.h"           /*File calling the objective functions*/
#include "select.h"             /*File for tournament selection*/
#include "crossover.h"          /*Uniform crossover*/
#include "mutation_SPM.h"   /*Neighborhood biased mutation*/
#include "keepaliven.h"         /*File for elitism and sharing scheme*/
#include "report.h"             /*Printing the report*/
#include "distance.h"           /*Euclidean distance calculation*/
#include "obj_CCrA.h"           /*Objective function for CCrA*/
#include "obj_SPM.h"          /*Objective function for SPM*/
#include "improvement.h"

/*Defining the population structures*/
population oldpop,
newpop,
matepop,
assignpop,
* old_pop_ptr,
* new_pop_ptr,
* mate_pop_ptr,
* assign_pop_ptr;

main()
{
	/*Output file pointers*/
	FILE
		* rep_ptr,
		* gen_ptr,
		* rep2_ptr,
		* time_ptr;

	/* Get the input from the file input.h*/
	/*input();*/

	int i, j, l, f, maxrank1, instance, feature, nn, nn2, value2, value3, closure, closure2, cl_size, cl_size_2, p1, p2, index[nvar], B[nvar], indi;
	double tot, value, point1[numF], point2[numF], dist, minDist, A[nvar], center[numF];

	/* Initialize lower and upper limits of genes*/
	for (closure = 0; closure < nvar; closure++)
	{
		lim_r[closure][0] = 0;
		lim_r[closure][1] = nvar - 1;
	}

	clock_t start_t, end_t;
	double total_t;
	start_t = clock();

	/*Opening the files*/
	char output_file_name[50], all_fit_file_name[50], rank_file_name[50], time_file_name[50];
	sprintf(output_file_name, "%s_output.txt", data_name);
	sprintf(all_fit_file_name, "%s_all_fitness.txt", data_name);
	sprintf(rank_file_name, "%s_ranks.txt", data_name);
	sprintf(time_file_name, "%s_time.txt", data_name);
	rep_ptr = fopen(output_file_name, "w");
	gen_ptr = fopen(all_fit_file_name, "w");
	rep2_ptr = fopen(rank_file_name, "w");
	time_ptr = fopen(time_file_name, "w");

	/* Read the data*/
	char data_file_name[50];
	FILE* data_ptr;
	sprintf(data_file_name, "%s.txt", data_name);
	data_ptr = fopen(data_file_name, "r");
	for (instance = 0; instance < instancenum; instance++)
	{
		for (feature = 0; feature < numF; feature++)
		{
			fscanf(data_ptr, "%lf", &value);
			Data[instance][feature] = value;
		}
	}
	fclose(data_ptr);

	/* Read the size of closures*/
	char closure_size_file_name[50];
	FILE* dataclosuresize_ptr;
	sprintf(closure_size_file_name, "%s_closure_size.txt", data_name);
	dataclosuresize_ptr = fopen(closure_size_file_name, "r");
	for (closure = 0; closure < nvar; closure++)
	{
		fscanf(dataclosuresize_ptr, "%i", &value2);
		closure_size[closure] = value2;
	}
	fclose(dataclosuresize_ptr);

	/* Read the NC closures*/
	char closure_file_name[50];
	FILE* data_closure_ptr;
	sprintf(closure_file_name, "%s_closures.txt", data_name);
	data_closure_ptr = fopen(closure_file_name, "r");
	for (closure = 0; closure < nvar; closure++)
	{
		cl_size = closure_size[closure];
		for (nn = 0; nn < cl_size; nn++)
		{
			fscanf(data_closure_ptr, "%i", &value3);
			closures[closure][nn] = value3 - 1;
		}
	}
	fclose(data_closure_ptr);

	/* Calculate the pairwise closure distances*/
	for (closure = 0; closure < nvar - 1; closure++)
	{
		cl_size = closure_size[closure];
		for (closure2 = closure + 1; closure2 < nvar; closure2++)
		{
			minDist = 10000000000000;
			cl_size_2 = closure_size[closure2];
			for (nn = 0; nn < cl_size; nn++)
			{
				p1 = closures[closure][nn];
				for (nn2 = 0; nn2 < cl_size_2; nn2++)
				{
					p2 = closures[closure2][nn2];
					for (feature = 0; feature < numF; feature++)
					{
						point1[feature] = Data[p1][feature];
						point2[feature] = Data[p2][feature];
					}
					dist = distance(point1, point2);
					if (dist < minDist) {
						minDist = dist;
					}
				}
				Distances[closure][closure2] = minDist;
				Distances[closure2][closure] = minDist;
			}
			Distances[closure][closure] = 0;
		}
	}

	/* Construct the closure neighborhood*/
	for (closure = 0; closure < nvar; closure++)
	{
		for (closure2 = 0; closure2 < nvar; closure2++)
		{
			index[closure2] = closure2;
		}
		for (closure2 = 0; closure2 < nvar; closure2++)
		{
			A[closure2] = Distances[closure][closure2];
		}

		double key;
		int key2;
		for (nn = 1; nn < nvar; nn++)
		{
			key = A[nn];
			key2 = index[nn];
			nn2 = nn - 1;
			while (nn2 >= 0 && A[nn2] > key)
			{
				A[nn2 + 1] = A[nn2];
				index[nn2 + 1] = index[nn2];
				nn2 = nn2 - 1;
			}
			A[nn2 + 1] = key;
			index[nn2 + 1] = key2;
		}
		for (closure2 = 0; closure2 < nvar; closure2++)
		{
			closure_neighborhood[closure][closure2] = index[closure2];
		}
	}

	/* Calculate closure centroids*/
	for (closure = 0; closure < nvar; closure++)
	{
		cl_size = closure_size[closure];
		for (nn = 0; nn < cl_size; nn++)
		{
			p1 = closures[closure][nn];
			for (feature = 0; feature < numF; feature++)
			{
				Centroids[closure][feature] = Centroids[closure][feature] + Data[p1][feature];
			}
		}
		for (feature = 0; feature < numF; feature++)
		{
			Centroids[closure][feature] = Centroids[closure][feature] / closure_size[closure];
		}
	}

	/* Calculate compactness of closures*/
	for (closure = 0; closure < nvar; closure++)
	{
		cl_size = closure_size[closure];
		for (nn = 0; nn < cl_size; nn++)
		{
			p1 = closures[closure][nn];
			for (feature = 0; feature < numF; feature++)
			{
				point1[feature] = Data[p1][feature];
				center[feature] = Centroids[closure][feature];
			}
			dist = distance(point1, center);
			Compactness[closure] = Compactness[closure] + dist;
		}
		Compactness[closure] = Compactness[closure] / closure_size[closure];
	}

	/* Preprocessing ends*/
	end_t = clock();
	total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
	fprintf(time_ptr, "Preprocesing, %f sec\n", total_t);

	old_pop_ptr = &(oldpop);
	assign_pop_ptr = &(assignpop);
	new_pop_ptr = &(newpop);

	/* Initialization of population*/
	init(old_pop_ptr);
	old_pop_ptr = &(oldpop);

	/* Initializing the Rank array having different individuals at a particular rank to zero*/
	for (j = 0; j < popsize; j++)
	{
		old_pop_ptr->rankno[j] = 0;
		new_pop_ptr->rankno[j] = 0;
	}
	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);

	/* Fitness function calculation*/
	decode_clusters(assign_pop_ptr, old_pop_ptr);
	assign_pop_ptr = &(assignpop);
	func(old_pop_ptr, assign_pop_ptr);
	assign_pop_ptr = &(assignpop);

	/* Initial population closure assignment*/
	char init_file_name[50];
	FILE* init_assign_ptr;
	sprintf(init_file_name, "%s_initial_pop.txt", data_name);
	init_assign_ptr = fopen(init_file_name, "w");
	for (j = 0; j < popsize; j++)
	{
		fprintf(init_assign_ptr, "\n%d ", j);
		for (closure = 0; closure < nvar; closure++)
		{
			fprintf(init_assign_ptr, "%d ", assign_pop_ptr->ind[j].xreal[closure]);
		}
	}

	fprintf(rep_ptr, "--------------------------------------------------\n");

	/*----------------------GENERATION STARTS HERE----------------------*/
	for (i = 0; i < gener; i++)
	{
		printf("Generation = %d\n", i + 1);
		old_pop_ptr = &(oldpop);
		mate_pop_ptr = &(matepop);
		assign_pop_ptr = &(assignpop);
		fprintf(gen_ptr, "#Generation No. --> %d\n", i + 1);
		fprintf(gen_ptr, "#Variable_vector  Fitness_vector Constraint_violation Overall_penalty\n");

		/*--------SELECT------------*/
		nselect(old_pop_ptr, mate_pop_ptr);
		mate_pop_ptr = &(matepop);
		new_pop_ptr = &(newpop);

		/*-------CROSSOVER-----------*/
		crossover(new_pop_ptr, mate_pop_ptr);
		decode_clusters(assign_pop_ptr, new_pop_ptr);
		assign_pop_ptr = &(assignpop);

		/*--------MUTATION-----------*/
		double sep = 0;
		for (j = 0; j < popsize; j++)
		{
			sep = SPM(assign_pop_ptr, j);
		}
		new_pop_ptr = &(newpop);
		assign_pop_ptr = &(assignpop);
		mutation(new_pop_ptr, assign_pop_ptr);

		/*----------FUNCTION EVALUATION-----------*/
		new_pop_ptr = &(newpop);
		assign_pop_ptr = &(assignpop);
		decode_clusters(assign_pop_ptr, new_pop_ptr);
		assign_pop_ptr = &(assignpop);
		func(new_pop_ptr, assign_pop_ptr);

		/*------SELECTION KEEPING FRONTS ALIVE--------*/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);
		assign_pop_ptr = &(assignpop);

		/*Elitism And Sharing Implemented*/
		keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);
		mate_pop_ptr = &(matepop);
		decode_clusters(assign_pop_ptr, mate_pop_ptr);
		assign_pop_ptr = &(assignpop);
		mate_pop_ptr = &(matepop);

		/* Time at the end of generation i*/
		end_t = clock();
		total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

		/*------------------REPORT PRINTING--------------------------------*/
		report(i, old_pop_ptr, mate_pop_ptr, rep_ptr, gen_ptr);

		/*Final population rank reporting and closure assignment*/
		if (i == gener - 1)
		{
			int rankmatrix[popsize];
			for (j = 0; j < popsize; j++)
			{
				rankmatrix[j] = 0;
			}
			for (j = 0; j < popsize; j++)
			{
				rankmatrix[j] = mate_pop_ptr->ind[j].rank;
			}

			/*Closure Assignment Reporting at the Final Population*/
			FILE* assign_ptr;
			char closure_assign_file_name[50];
			sprintf(closure_assign_file_name, "%s_cluster_assignment_%d.txt", data_name, i + 1);
			assign_ptr = fopen(closure_assign_file_name, "w");
			clusassign(assign_pop_ptr, assign_ptr, rankmatrix);
			fclose(assign_ptr);
		}

		/*----------------Rank Ratio Calculation------------------------*/
		new_pop_ptr = &(matepop);
		old_pop_ptr = &(oldpop);

		/*Finding the greater maxrank among the two populations*/
		if (old_pop_ptr->maxrank > new_pop_ptr->maxrank)
			maxrank1 = old_pop_ptr->maxrank;
		else
			maxrank1 = new_pop_ptr->maxrank;

		fprintf(rep2_ptr, "--------RANK AT GENERATION %d--------------\n", i + 1);
		fprintf(rep2_ptr, "Rank old ranks   new ranks     rankratio\n");

		for (j = 0; j < maxrank1; j++)
		{
			/*Sum of the no of individuals at any rank in old population and the new population*/
			tot = (old_pop_ptr->rankno[j]) + (new_pop_ptr->rankno[j]);
			/*Finding the rank ratio for new population at this rank*/
			new_pop_ptr->rankrat[j] = (new_pop_ptr->rankno[j]) / tot;
			/*Printing this rank ratio to a file called ranks.dat*/
			fprintf(rep2_ptr, " %d\t  %d\t\t %d\t %f\n", j + 1, old_pop_ptr->rankno[j], new_pop_ptr->rankno[j], new_pop_ptr->rankrat[j]);
		}
		fprintf(rep2_ptr, "-----------------Rank Ratio-------------------\n");
		/*=======Copying the new population to old population======*/
		old_pop_ptr = &(oldpop);
		new_pop_ptr = &(matepop);

		for (j = 0; j < popsize; j++)
		{
			old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
			new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);

			/*For Real Coded GA copying of the chromosomes*/
			if (nvar > 0)
			{
				for (l = 0; l < nvar; l++)
					old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
			}

			/*Copying the fitness vector */
			for (l = 0; l < nfunc; l++)
				old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

			/*Copying the dummy fitness*/
			old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

			/*Copying the rank of the individuals*/
			old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

			/*Copying the error of the individual*/
			old_pop_ptr->ind_ptr->error = new_pop_ptr->ind_ptr->error;

			/*Copying the flag of the individuals*/
			old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
		}
		maxrank1 = new_pop_ptr->maxrank;

		/*Copying the array having the record of the individual at different ranks */
		for (l = 0; l < popsize; l++)
		{
			old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
		}

		/*Copying the maxrank */
		old_pop_ptr->maxrank = new_pop_ptr->maxrank;

		/*Printing the fitness record for generation i in a file*/
		if (i == gener - 1)
		{
			FILE* end_ptr, * g_var;
			char final_fit_file[50], final_var_file[50];
			sprintf(final_fit_file, "%s_final_fitness_%d.txt", data_name, i + 1);
			sprintf(final_var_file, "%s_final_var_%d.txt", data_name, i + 1);
			end_ptr = fopen(final_fit_file, "w");
			g_var = fopen(final_var_file, "w");
			fprintf(time_ptr, "Generation = %d, %f sec\n", i + 1, total_t);

			for (f = 0; f < popsize; f++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);
				if ((old_pop_ptr->ind_ptr->error <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))
				{
					for (l = 0; l < nfunc; l++)
						fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->fitness[l]);
					fprintf(end_ptr, "\n");
					if (nvar > 0)
					{
						for (l = 0; l < nvar; l++)
						{
							fprintf(g_var, "%i\t", old_pop_ptr->ind_ptr->xreal[l]);
						}
						fprintf(g_var, "  ");
					}
					fprintf(g_var, "\n");
				}
			}
			fclose(end_ptr);
			fclose(g_var);
		}


		/*Improvement patch*/
		if (i == gener - 1)
		{
			old_pop_ptr = &(oldpop);

			patch(old_pop_ptr, assign_pop_ptr);
			decode_clusters(assign_pop_ptr, old_pop_ptr);

			/*----------FUNCTION EVALUATION-----------*/
			assign_pop_ptr = &(assignpop);
			func(old_pop_ptr, assign_pop_ptr);
			old_pop_ptr = &(oldpop);

			/* Time after improvement*/
			end_t = clock();
			total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

			int rankmatrix3[popsize];
			for (j = 0; j < popsize; j++)
			{
				rankmatrix3[j] = 0;
			}
			for (j = 0; j < popsize; j++)
			{
				rankmatrix3[j] = old_pop_ptr->ind[j].rank;
			}
			/*Closure Assignment Reporting after Improvement*/
			FILE* assign_ptr;
			char closure_assign_file_name[80];
			sprintf(closure_assign_file_name, "%s_cluster_assignment_after_improvement.txt", data_name);
			assign_ptr = fopen(closure_assign_file_name, "w");

			for (j = 0; j < popsize; j++)
			{
				if (rankmatrix3[j] == 1)
				{
					fprintf(assign_ptr, "\n%d ", j);
					for (closure = 0; closure < nvar; closure++)
					{
						fprintf(assign_ptr, "%d ", assign_pop_ptr->ind[j].xreal[closure]);
					}
				}
			}
			fclose(assign_ptr);

			FILE* end_ptr, * g_var;
			char final_fit_file[80], final_var_file[80];
			sprintf(final_fit_file, "%s_final_fitness_after_improvement.txt", data_name);
			sprintf(final_var_file, "%s_final_var_after_improvement.txt", data_name);
			end_ptr = fopen(final_fit_file, "w");
			g_var = fopen(final_var_file, "w");
			fprintf(time_ptr, "After improvement, %f sec\n", total_t);

			old_pop_ptr = &(oldpop);
			for (f = 0; f < popsize; f++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);
				if ((old_pop_ptr->ind_ptr->error <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))
				{
					for (l = 0; l < nfunc; l++)
						fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->fitness[l]);
					fprintf(end_ptr, "\n");
					if (nvar > 0)
					{
						for (l = 0; l < nvar; l++)
						{
							fprintf(g_var, "%i\t", old_pop_ptr->ind_ptr->xreal[l]);
						}
						fprintf(g_var, "  ");
					}
					fprintf(g_var, "\n");
				}
			}
			fclose(end_ptr);
			fclose(g_var);
		}
	}

	/*Closing the files*/
	fclose(rep_ptr);
	fclose(gen_ptr);
	fclose(rep2_ptr);
	fclose(time_ptr);
	return 0;
}