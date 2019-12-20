#include <time.h>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MinimumSpanningTree.h"
#include "InitRandPopulation.h"
#include "Parameters.h";
#include "Functions.h";
#include "select.h";
//#include "keepaliven.h";

using namespace std;


int main(int, char**)
{
	int STOP;
	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);

	srand(time(NULL));

	FILE
		* value,
		* coordX,
		* coordY;

	errno_t err;

	fopen_s(&value, "demand_value.txt", "r");
	fopen_s(&coordX, "demand_coordX.txt", "r");
	fopen_s(&coordY, "demand_coordY.txt", "r");
	/*Opening the files*/

	for (int i = 0; i < numDemand; i++)
	{
		
		demand_ptr = &(demandSet);
		fscanf(value, "%d\n", &(demand_ptr->Value[i]));
		fscanf(coordX, "%f\n", &(demand_ptr->CoordX[i]));
		fscanf(coordY, "%f\n", &(demand_ptr->CoordY[i]));
		//printf("%f\n", demand_ptr->CoordX[i]);
	}
	fclose(value);
	fclose(coordX);
	fclose(coordY);

	old_pop_ptr = &(oldpop);
	initPopulation(old_pop_ptr);		//Generate initial population
	
	old_pop_ptr = &(oldpop);
	findCoverage(old_pop_ptr);			//Find coverage objective
	
	old_pop_ptr = &(oldpop);
	find_numDC(old_pop_ptr);			//Find cost objective

	for (int j = 0; j < popSize; j++)
	{
		/*Initializing the Rank array having different individuals
	  at a particular  rank to zero*/
		old_pop_ptr->rankno[j] = 0;
		new_pop_ptr->rankno[j] = 0;
	}

	old_pop_ptr = &(oldpop);

	ranking(old_pop_ptr);
	old_pop_ptr = &(oldpop);
	for (int i = 0; i < popSize; i++)
	{
		printf("%d \n", old_pop_ptr->ind[i].rank);			// Print rank of individuals in the initial population
	}



	 /********************************************************************/
  /*----------------------GENERATION STARTS HERE----------------------*/
	for (int i = 0; i < gener; i++)
	{
		old_pop_ptr = &(oldpop);
		mate_pop_ptr = &(matepop);

		/*--------SELECT----------------*/
		nselect(old_pop_ptr, mate_pop_ptr);

		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

			///// CROSSOVER EKLENECEK //////

		crossover(mate_pop_ptr,new_pop_ptr);			/* if |rank==1| > 10 then size of new pop_pop_ptr can't be enough, 
														as it is popSize currently.*/

		new_pop_ptr = &(newpop);
		find_numFac(new_pop_ptr);

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);			//Find coverage objective

		new_pop_ptr = &(newpop);
		find_numDC(new_pop_ptr);			//Find cost objective

		new_pop_ptr = &(newpop);
		for (int i = 0; i < 20; i++)
		{
			//printf("%d\n",new_pop_ptr->ind[i].numRS); //0 tane facility'si olan sonuçlar dönüyor!!!!!
		}

			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);

			/*Elitism And Sharing Implemented*/
			//keepalive(old_pop_ptr, new_pop_ptr, mate_pop_ptr, i + 1);

			mate_pop_ptr = &(matepop);

		//	/*------------------REPORT PRINTING--------------------------------*/
		//	report(i, old_pop_ptr, mate_pop_ptr, rep_ptr, gen_ptr, lastit);

		//	/*==================================================================*/

		//	/*----------------Rank Ratio Calculation------------------------*/
		//	new_pop_ptr = &(matepop);
		//	old_pop_ptr = &(oldpop);

		//	/*Finding the greater maxrank among the two populations*/

		//	if (old_pop_ptr->maxrank > new_pop_ptr->maxrank)
		//		maxrank1 = old_pop_ptr->maxrank;
		//	else
		//		maxrank1 = new_pop_ptr->maxrank;

		//	fprintf(rep2_ptr, "--------RANK AT GENERATION %d--------------\n", i + 1);
		//	fprintf(rep2_ptr, "Rank old ranks   new ranks     rankratio\n");

		//	for (j = 0; j < maxrank1; j++)
		//	{
		//		/*Sum of the no of individuals at any rank in old population
		//		  and the new populaion*/

		//		tot = (old_pop_ptr->rankno[j]) + (new_pop_ptr->rankno[j]);

		//		/*Finding the rank ratio for new population at this rank*/

		//		new_pop_ptr->rankrat[j] = (new_pop_ptr->rankno[j]) / tot;

		//		/*Printing this rank ratio to a file called ranks.dat*/

		//		fprintf(rep2_ptr, " %d\t  %d\t\t %d\t %f\n", j + 1, old_pop_ptr->rankno[j], new_pop_ptr->rankno[j], new_pop_ptr->rankrat[j]);

		//	}

		//	fprintf(rep2_ptr, "-----------------Rank Ratio-------------------\n");
		//	/*==================================================================*/

		//	/*=======Copying the new population to old population======*/

		//	old_pop_ptr = &(oldpop);
		//	new_pop_ptr = &(matepop);

		//	for (j = 0; j < popsize; j++)
		//	{
		//		old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
		//		new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[j]);
		//		if (nchrom > 0)
		//		{
		//			/*For Binary GA copying of the chromosome*/

		//			for (l = 0; l < chrom; l++)
		//				old_pop_ptr->ind_ptr->genes[l] = new_pop_ptr->ind_ptr->genes[l];

		//			for (l = 0; l < nchrom; l++)
		//				old_pop_ptr->ind_ptr->xbin[l] = new_pop_ptr->ind_ptr->xbin[l];
		//		}
		//		if (nvar > 0)
		//		{
		//			/*For Real Coded GA copying of the chromosomes*/
		//			for (l = 0; l < nvar; l++)
		//				old_pop_ptr->ind_ptr->xreal[l] = new_pop_ptr->ind_ptr->xreal[l];
		//		}

		//		/*Copying the fitness vector */
		//		for (l = 0; l < nfunc; l++)
		//			old_pop_ptr->ind_ptr->fitness[l] = new_pop_ptr->ind_ptr->fitness[l];

		//		/*Copying the dummy fitness*/
		//		old_pop_ptr->ind_ptr->cub_len = new_pop_ptr->ind_ptr->cub_len;

		//		/*Copying the rank of the individuals*/
		//		old_pop_ptr->ind_ptr->rank = new_pop_ptr->ind_ptr->rank;

		//		/*Copying the error and constraints of the individual*/

		//		old_pop_ptr->ind_ptr->error = new_pop_ptr->ind_ptr->error;
		//		for (l = 0; l < ncons; l++)
		//		{
		//			old_pop_ptr->ind_ptr->constr[l] = new_pop_ptr->ind_ptr->constr[l];
		//		}

		//		/*Copying the flag of the individuals*/
		//		old_pop_ptr->ind_ptr->flag = new_pop_ptr->ind_ptr->flag;
		//	}   // end of j

		//	maxrank1 = new_pop_ptr->maxrank;

		//	/*Copying the array having the record of the individual
		//  at different ranks */
		//	for (l = 0; l < popsize; l++)
		//	{
		//		old_pop_ptr->rankno[l] = new_pop_ptr->rankno[l];
		//	}

		//	/*Copying the maxrank */
		//	old_pop_ptr->maxrank = new_pop_ptr->maxrank;

		//	/*Printing the fitness record for last generation in a file last*/
		//	if (i == gener - 1)
		//	{  // for the last generation 
		//		old_pop_ptr = &(matepop);
		//		for (f = 0; f < popsize; f++) // for printing
		//		{
		//			old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[f]);

		//			if ((old_pop_ptr->ind_ptr->error <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))  // for all feasible solutions and non-dominated solutions
		//			{
		//				for (l = 0; l < nfunc; l++)
		//					fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->fitness[l]);
		//				for (l = 0; l < ncons; l++)
		//				{
		//					fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->constr[l]);
		//				}
		//				if (ncons > 0)
		//					fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->error);
		//				fprintf(end_ptr, "\n");

		//				if (nvar > 0)
		//				{
		//					for (l = 0; l < nvar; l++)
		//					{
		//						fprintf(g_var, "%f\t", old_pop_ptr->ind_ptr->xreal[l]);
		//					}
		//					fprintf(g_var, "  ");
		//				}

		//				if (nchrom > 0)
		//				{
		//					for (l = 0; l < nchrom; l++)
		//					{
		//						fprintf(g_var, "%f\t", old_pop_ptr->ind_ptr->xbin[l]);
		//					}
		//				}
		//				fprintf(g_var, "\n");
		//			}  // feasibility check
		//		} // end of f (printing)

		//	} // for the last generation
		//}  // end of i 

	 // /*                   Generation Loop Ends                                */
	 // /************************************************************************/

		//

		///*for (int i = 0; i < 5; i++)
		//{
		//	for (int j = 0; j <3; j++)
		//	{
		//		cout << old_pop_ptr->ind_ptr << endl;
		//	}
		//	
		//}
		//*/
	
cout << "END" << endl;

		cin >> STOP;

	}

	return 0;
}