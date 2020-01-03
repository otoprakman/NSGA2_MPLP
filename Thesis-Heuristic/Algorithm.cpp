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
#include "keepaliven.h";

using namespace std;


int main(int, char**)
{
	int STOP;
	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);
	mate_pop_ptr = &(matepop);

	srand(time(NULL));
	
	clock_t start, end;


	FILE
		* value,
		* coordX,
		* coordY,
		* writer,
		*writeCost,
		*writeCoverage,
		*writeSumFacility; 

	FILE
		* selectTime,
		* crossoverTime,
		* findValueTime,
		* keepAliveTime,
		* deleteOldTime,
		* copyNewGenTime;


	/* open the file for writing*/
	writer = fopen("FacilityNumberAfterCrossover.txt", "w");
	writeCost = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\HeuristicCost.txt", "w");
	writeCoverage = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\HeuristicCoverage.txt", "w");
	writeSumFacility = fopen("SumFacility.txt", "w");
	
	
	selectTime = fopen("selectTime.txt", "w");
	crossoverTime = fopen("crossoverTime.txt", "w");
	findValueTime = fopen("findValueTime.txt", "w");
	keepAliveTime = fopen("keepAliveTime.txt", "w");
	deleteOldTime = fopen("deleteOldTime.txt", "w");
	copyNewGenTime = fopen("copyNewGenTime.txt", "w");

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

	printf("**** REPORTS INITIAL POPULATION ****\n");

	/*for (int i = 0; i < popSize; i++)		
	{
		printf("%d- RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d - %d.RankSize:%d\n", i + 1, old_pop_ptr->ind[i].numRS, old_pop_ptr->ind[i].numDC,
			old_pop_ptr->ind[i].numFac, old_pop_ptr->ind[i].fitness[0], old_pop_ptr->ind[i].fitness[1], old_pop_ptr->ind[i].rank,
			i + 1, old_pop_ptr->rankno[i]);
	}*/
	printf("Maximum Rank in Initial Population:%d\n", old_pop_ptr->maxrank);

	int maxrank1;

	/*for (int i = 0; i < maxpop; i++)
	{
		printf("%d-Crowding distances: %f\n", i+1, old_pop_ptr->ind[i].cub_len);

	}*/

																					//********** NOTLAR *********//
	 /********************************************************************/			
    /*----------------------GENERATION STARTS HERE----------------------*/			
   /********************************************************************/			


	for (int generation = 0; generation < generationNum; generation++)
	{
		old_pop_ptr = &(oldpop);		
		mate_pop_ptr = &(matepop);

		start = clock();
		
		//for (int i = 0; i < popSize; i++)
		//{
		//	printf("%d\n", old_pop_ptr->ind[i].numFac);
		//}
		/*--------SELECT----------------*/
		nselect(old_pop_ptr, mate_pop_ptr);
		//printf("******************\n");
		//for (int i = 0; i < 2*popSize; i++)
		//{
		//	printf("%d\n", mate_pop_ptr->ind[i].numFac);
		//}

		end = clock();

		printExecTime(start, end, selectTime);

		/*for (int i = 0; i < 2*popSize; i++)
		{
			printf("%d-(MATE) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d\n", i + 1, mate_pop_ptr->ind[i].numRS, mate_pop_ptr->ind[i].numDC,
				mate_pop_ptr->ind[i].numFac, mate_pop_ptr->ind[i].fitness[0], mate_pop_ptr->ind[i].fitness[1]);
		}*/
		//for (int i = 0; i < 50; i++)
		//{
		//	printf("NEWPOP:RANK:%d, %f\n", mate_pop_ptr->ind[0].numFac, mate_pop_ptr->ind[0].facilitySet[i].CoordX);
		//}

		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

	/*	for (int i = 0; i < 2*popSize; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				printf("MATE: %d-RS:%d DC:%d Fac:%d Cov:%d Cost:%d X:%f--Y:%f\n",
					i, mate_pop_ptr->ind[i].numRS, mate_pop_ptr->ind[i].numDC,
					mate_pop_ptr->ind[i].numFac, mate_pop_ptr->ind[i].fitness[1],
					mate_pop_ptr->ind[i].fitness[0], mate_pop_ptr->ind[i].facilitySet[j].CoordX,
					mate_pop_ptr->ind[i].facilitySet[j].CoordY);
			}
		}
		*/

		start = clock();

		crossover(mate_pop_ptr,new_pop_ptr,writer);	
		
		/* close the file*/
		end = clock();

		printExecTime(start, end, crossoverTime);

		

		///////DELETE
		for (int i = 0; i < popSize; i++)
		{
			for (int j = 0; j < max_numFac; j++)
			{
				mate_pop_ptr->ind[i].facilitySet[j].CoordX = 0.0;
				mate_pop_ptr->ind[i].facilitySet[j].CoordY = 0.0;
			}
		}

		start = clock();
		
		new_pop_ptr = &(newpop);
		find_numFac(new_pop_ptr);
		int	sumFacility=0;
		for (int i=0, j= 0; i < popSize, j < 2*popSize; i++, j+=2)
		{
			sumFacility += new_pop_ptr->ind[i].numFac;
			float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

			fprintf(writer, "%f\n", 
				ratio);
		}

		fprintf(writeSumFacility, "%d\n", sumFacility);

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);			//Find coverage objective

		new_pop_ptr = &(newpop);
		find_numDC(new_pop_ptr);			//Find cost objective
		
		end = clock();

		printExecTime(start, end, findValueTime);

		new_pop_ptr = &(newpop);
		/*for (int i = 0; i < popSize; i++)
		{
			printf("%d-(NEW) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d\n", i + 1, new_pop_ptr->ind[i].numRS, new_pop_ptr->ind[i].numDC,
				new_pop_ptr->ind[i].numFac, new_pop_ptr->ind[i].fitness[0], new_pop_ptr->ind[i].fitness[1]);
		}*/
			/*-------------------SELECTION KEEPING FRONTS ALIVE--------------*/
			old_pop_ptr = &(oldpop);
			new_pop_ptr = &(newpop);
			mate_pop_ptr = &(matepop);
			last_pop_ptr = &(lastpop);

			/*for (int i = 0; i < popSize; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					printf("NEW: %d-RS:%d DC:%d Fac:%d Cov:%d Cost:%d X:%f--Y:%f\n",
						i, new_pop_ptr->ind[i].numRS, new_pop_ptr->ind[i].numDC,
						new_pop_ptr->ind[i].numFac, new_pop_ptr->ind[i].fitness[1],
						new_pop_ptr->ind[i].fitness[0], new_pop_ptr->ind[i].facilitySet[j].CoordX,
						new_pop_ptr->ind[i].facilitySet[j].CoordY);
				}
			}*/

			start = clock();
			/*Elitism And Sharing Implemented*/
			keepalive(old_pop_ptr, new_pop_ptr, last_pop_ptr, generation + 1); 
			end = clock();

			printExecTime(start, end, keepAliveTime);

			start = clock();

			///////DELETE
			for (int i = 0; i < popSize; i++)
			{
				for (int j = 0; j < max_numFac; j++)
				{
					old_pop_ptr->ind[i].facilitySet[j].CoordX = 0.0;
					old_pop_ptr->ind[i].facilitySet[j].CoordY = 0.0;
				}
			}

			///////DELETE
			for (int i = 0; i < popSize; i++)
			{
				for (int j = 0; j < max_numFac; j++)
				{
					new_pop_ptr->ind[i].facilitySet[j].CoordX = 0.0;
					new_pop_ptr->ind[i].facilitySet[j].CoordY = 0.0;
				}
			}
			end = clock();

			printExecTime(start, end, deleteOldTime);

			last_pop_ptr = &(lastpop);
			if (generation==generationNum-1)
			{
				for (int i = 0; i < popSize; i++)
				{
					if (last_pop_ptr->ind[i].rank==1)
					{
						printf("%d-(LAST) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d- %d.RankSize:%d\n", i + 1, last_pop_ptr->ind[i].numRS, last_pop_ptr->ind[i].numDC,
							last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].fitness[0], last_pop_ptr->ind[i].fitness[1], last_pop_ptr->ind[i].rank,
							i + 1, last_pop_ptr->rankno[i]);
						fprintf(writeCost, "%d\n", -1*last_pop_ptr->ind[i].fitness[0]);
						fprintf(writeCoverage, "%d\n", last_pop_ptr->ind[i].fitness[1]);
					}

				} //Ranksize'ı sürekli üzerine yazdığım için hatalı gözüken sonuçlar verebilir. Kümülatif toplamları popSize'a eşit olana kadar doğru veriyor.

			}
				
			
			//for (int m = 0; m < 2 * popSize; m++)
			//{
			//	//printf("Crowding: %f\n", );

			//}

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

			/*=======Copying the new population to old population======*/

			old_pop_ptr = &(oldpop);
			last_pop_ptr = &(lastpop);

			start = clock();

			for (int j = 0; j < popSize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				last_pop_ptr->ind_ptr = &(last_pop_ptr->ind[j]);
			
				/*Copy faciltySet for each individual*/

				for (int l = 0; l < last_pop_ptr->ind[j].numFac; l++)
					old_pop_ptr->ind_ptr->facilitySet[l] = last_pop_ptr->ind_ptr->facilitySet[l];

				old_pop_ptr->ind_ptr->numDC = last_pop_ptr->ind_ptr->numDC;

				old_pop_ptr->ind_ptr->numRS = last_pop_ptr->ind_ptr->numRS;

				old_pop_ptr->ind_ptr->numFac = last_pop_ptr->ind_ptr->numFac;

				/*Copying the fitness vector */
				for (int l = 0; l < maxfun; l++)
					old_pop_ptr->ind_ptr->fitness[l] = last_pop_ptr->ind_ptr->fitness[l];

				/*Copying the dummy fitness*/
				old_pop_ptr->ind_ptr->cub_len = last_pop_ptr->ind_ptr->cub_len;

				/*Copying the rank of the individuals*/
				old_pop_ptr->ind_ptr->rank = last_pop_ptr->ind_ptr->rank;

				/*Copying the flag of the individuals*/
				old_pop_ptr->ind_ptr->flag = last_pop_ptr->ind_ptr->flag;
				old_pop_ptr->rankno[j] = last_pop_ptr->rankno[j];
			}   // end of j

			old_pop_ptr->maxrank = last_pop_ptr->maxrank;
			maxrank1 = last_pop_ptr->maxrank;


			///////DELETE
			for (int i = 0; i < popSize; i++)
			{
				for (int j = 0; j < max_numFac; j++)
				{
					last_pop_ptr->ind[i].facilitySet[j].CoordX = 0.0;
					last_pop_ptr->ind[i].facilitySet[j].CoordY = 0.0;
				}
			}

			end = clock();

			printExecTime(start, end, copyNewGenTime);

			///*Copying the array having the record of the individual
		 // at different ranks */
			//for (int l = 0; l < popSize; l++)
			//{
			//	old_pop_ptr->rankno[l] = last_pop_ptr->rankno[l];
			//}

			///*Copying the maxrank */


			///*Printing the fitness record for last generation in a file last*/
			//if (i == gener - 1)
			//{  // for the last generation 
			//	mate_pop_ptr = &(matepop);
			//	for (f = 0; f < popSize; f++) // for printing
			//	{
			//		old_pop_ptr->ind_ptr = &(mate_pop_ptr->ind[f]);

			//		if ((old_pop_ptr->ind_ptr->error <= 0.0) && (old_pop_ptr->ind_ptr->rank == 1))  // for all feasible solutions and non-dominated solutions
			//		{
			//			for (l = 0; l < nfunc; l++)
			//				fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->fitness[l]);
			//			for (l = 0; l < ncons; l++)
			//			{
			//				fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->constr[l]);
			//			}
			//			if (ncons > 0)
			//				fprintf(end_ptr, "%f\t", old_pop_ptr->ind_ptr->error);
			//			fprintf(end_ptr, "\n");

			//			if (nvar > 0)
			//			{
			//				for (l = 0; l < nvar; l++)
			//				{
			//					fprintf(g_var, "%f\t", old_pop_ptr->ind_ptr->xreal[l]);
			//				}
			//				fprintf(g_var, "  ");
			//			}

			//			if (nchrom > 0)
			//			{
			//				for (l = 0; l < nchrom; l++)
			//				{
			//					fprintf(g_var, "%f\t", old_pop_ptr->ind_ptr->xbin[l]);
			//				}
			//			}
			//			fprintf(g_var, "\n");
			//		} // feasibility check
			//	} // end of f (printing)

			/*if (generation == generationNum-1)
			{
				for (int i = 0; i < popSize; i++)
				{
						printf("LAST: %d-RS:%d DC:%d Fac:%d Cov:%d Cost:%d\n",
							i, old_pop_ptr->ind[i].numRS, old_pop_ptr->ind[i].numDC,
							old_pop_ptr->ind[i].numFac, old_pop_ptr->ind[i].fitness[1],
							old_pop_ptr->ind[i].fitness[0]);
				}
			}*/
				
			printf("END-OF GENERATION: %d\n", generation + 1);
	} // for the last generation
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
	fclose(writer);
	fclose(writeCost);
	fclose(writeCoverage);
	fclose(writeSumFacility);

	fclose(selectTime);
	fclose(crossoverTime);
	fclose(findValueTime);
	fclose(keepAliveTime);
	fclose(deleteOldTime);
	fclose(copyNewGenTime);

	cin >> STOP;
	return 0;

}