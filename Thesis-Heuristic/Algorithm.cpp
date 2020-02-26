#include <time.h>
#include <ctime> 
#include <math.h>
#include "KruskalMST.h"
#include "PrimsMST.h"
#include "InitRandPopulation.h"
#include "Parameters.h";
#include "Functions.h";
#include "select.h";
#include "keepaliven.h";
#include "ReadWriteFiles.h";

using namespace std;


int main(int, char**)
{
	int STOP;

	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);
	mate_pop_ptr = &(matepop);

	srand(time(NULL));
	
	clock_t start, end;

	readWrite();						//Read Demand attributes from the file

	old_pop_ptr = &(oldpop);
	initPopulation(old_pop_ptr);		//Generate initial population
	
	old_pop_ptr = &(oldpop);
	findCoverage(old_pop_ptr);			//Find coverage objective

	old_pop_ptr = &(oldpop);
	find_numDCP(old_pop_ptr);			//Find cost objective

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

	}
	printf("Maximum Rank in Initial Population:%d\n", old_pop_ptr->maxrank);*/

	int maxrank1;

	//for (int i = 0; i < maxpop; i++)
	//{
	//	printf("%d-Crowding distances: %f\n", i+1, old_pop_ptr->ind[i].cub_len);
	//}
																					//********** NOTLAR *********//
	 /********************************************************************/			
    /*----------------------GENERATION STARTS HERE----------------------*/			
   /********************************************************************/			


	for (int generation = 0; generation < generationNum; generation++)
	{
		old_pop_ptr = &(oldpop);		
		mate_pop_ptr = &(matepop);

		start = clock();
		
		counter += 1;

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
	


		/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
		for (int i = 0; i < popSize; i++)
		{
			mate_pop_ptr->ind[i].facilitySet.clear();
		}

		new_pop_ptr = &(newpop);
		find_numFac(new_pop_ptr);

		int	sumFacility=0;
		int max = 0;
		int min = 999999;

		for (int i=0, j= 0; i < popSize, j < 2*popSize; i++, j+=2)
		{
			sumFacility += new_pop_ptr->ind[i].numFac;
			if (new_pop_ptr->ind[i].numFac > max)
			{
				max = new_pop_ptr->ind[i].numFac;
			}
			if (new_pop_ptr->ind[i].numFac < min)
			{
				min = new_pop_ptr->ind[i].numFac;
			}
			//float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

			//fprintf(writer, "%f\n", ratio);
		}

		fprintf(writeSumFacility, "%d\t %d\t %d\n", sumFacility, max, min);

		start = clock();

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);			//Find coverage objective

		new_pop_ptr = &(newpop);
		find_numDCP(new_pop_ptr);			//Find cost objective

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
			

			int	sumFacility2 = 0;
			int max2 = 0;
			int min2 = 999999;

			for (int i = 0, j = 0; i < popSize, j < 2 * popSize; i++, j += 2)
			{
				sumFacility2 += last_pop_ptr->ind[i].numFac;
				if (last_pop_ptr->ind[i].numFac > max2)
				{
					max2 = last_pop_ptr->ind[i].numFac;
				}
				if (last_pop_ptr->ind[i].numFac < min2)
				{
					min2 = last_pop_ptr->ind[i].numFac;
				}
				//float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

				//fprintf(writer, "%f\n", ratio);
			}

			fprintf(writeSumFacilityAfterSelection, "%d\t %d\t %d\n", sumFacility2, max2, min2);

			end = clock();

			printExecTime(start, end, keepAliveTime);

			start = clock();

			/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
			for (int i = 0; i < popSize; i++)
			{
				old_pop_ptr->ind[i].facilitySet.clear();				
			}
			
			/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
			for (int i = 0; i < popSize; i++)
			{
				new_pop_ptr->ind[i].facilitySet.clear();				
			}
			end = clock();

			printExecTime(start, end, deleteOldTime);

			last_pop_ptr = &(lastpop);
			
			//Print Last Generation Solutions

			if (generation>=generationNum-1)
			{
				int count = 0;
				for (int i = 0; i < popSize; i++)
				{
					if (last_pop_ptr->ind[i].rank==1)
					{
						count += 1;
						printf("%d-(LAST) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d- %d.RankSize:%d\n", count, last_pop_ptr->ind[i].numRS, last_pop_ptr->ind[i].numDC,
							last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].fitness[0], last_pop_ptr->ind[i].fitness[1], last_pop_ptr->ind[i].rank,
							i + 1, last_pop_ptr->rankno[i]);

						fprintf(writeCost, "%d\n", -1 * last_pop_ptr->ind[i].fitness[0]);
						fprintf(writeCoverage, "%d\n", last_pop_ptr->ind[i].fitness[1]);
						fprintf(writenumFac, "%d\n", last_pop_ptr->ind[i].numFac);

						for (int j = 0; j < last_pop_ptr->ind[i].numFac; j++)
						{
							fprintf(writeCoordX, "%f\n", last_pop_ptr->ind[i].facilitySet[j].CoordX);
							fprintf(writeCoordY, "%f\n", last_pop_ptr->ind[i].facilitySet[j].CoordY);

						}
					}

				} //Ranksize'ı sürekli üzerine yazdığım için hatalı gözüken sonuçlar verebilir. Kümülatif toplamları popSize'a eşit olana kadar doğru veriyor.
				printf("******************************************\n");
			}

		//	/*==================================================================*/

			/*=======Copying the New Population to Old Population======*/

			old_pop_ptr = &(oldpop);
			last_pop_ptr = &(lastpop);

			start = clock();

			for (int j = 0; j < popSize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				last_pop_ptr->ind_ptr = &(last_pop_ptr->ind[j]);
			
				/*Copy faciltySet for each individual*/

				old_pop_ptr->ind_ptr->facilitySet = last_pop_ptr->ind_ptr->facilitySet;

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


			/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
			for (int i = 0; i < popSize; i++)
			{
				last_pop_ptr->ind[i].facilitySet.clear();
				last_pop_ptr->ind[i].facilitySet.clear();				
			}

			end = clock();

			printExecTime(start, end, copyNewGenTime);
				
			printf("END-OF GENERATION: %d\n", generation + 1);
	} // for the last generation

	 // /*                   Generation Loop Ends                                */
	 // /************************************************************************/
	
	cout << "END" << endl;
	fclose(writer);
	fclose(writeCost);
	fclose(writeCoverage);
	fclose(writeSumFacility);
	fclose(writenumFac);
	fclose(writeCoordX);
	fclose(writeCoordY);
	fclose(writeSumFacilityAfterSelection);


	fclose(selectTime);
	fclose(crossoverTime);
	fclose(findValueTime);
	fclose(keepAliveTime);
	fclose(deleteOldTime);
	fclose(copyNewGenTime);
	if (Plot)
	{
		RPath = string("C:\\\"Program Files\"\\R\\R-3.6.2\\bin\\R.exe < C:\\\"Users\"\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\MIP_Sol1_Sol2_Compare.R --no-save");
		system(RPath.c_str());
	}
	cin >> STOP;
	return 0;

}