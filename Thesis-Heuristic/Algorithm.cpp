#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <ctime> 
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>

//#include "ConvexHull_Chan.h";

#include "Parameters.h";
#include "ReadWriteFiles.h";
#include "Functions.h";

#include "InitialPopulation.h";
#include "ParentSelection.h";
#include "Crossover.h";
#include "isInsideHull.h";
#include "ConvexHullGraham.h";
#include "Mutation.h";
#include "PrimsMST.h";
#include "keepaliven.h";



int main(int, char**)
{

	int STOP;
	starting = clock();

	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);
	mate_pop_ptr = &(matepop);

	srand(time(NULL));

	readWrite();						//Read Demand attributes from the file

	startingAll = clock();

	old_pop_ptr = &(oldpop);
	//externalInitPop(old_pop_ptr);		//Generate initial population
	initPopulation(old_pop_ptr);		//Generate initial population
	cout << "Generating Initial Population: COMPLETED" << std::endl;
	
	ending = clock();

	initTime = ExecTime(starting, ending);
	/////*************************************//////

	old_pop_ptr = &(oldpop);
	findCoverage(old_pop_ptr);			//Find coverage objective
	cout << "Find Coverage for Initial Population: COMPLETED" << std::endl;

	//////*****************************/////

	old_pop_ptr = &(oldpop);
	findCost(old_pop_ptr);				//Find cost objective
	cout << "Find Cost for Initial Population: COMPLETED" << std::endl;

	//////*****************************/////

	for (int j = 0; j < popSize; j++)
	{
		/*Initializing the Rank array having different individuals
	  at a particular  rank to zero*/
		old_pop_ptr->rankno[j] = 0;
		new_pop_ptr->rankno[j] = 0;
	}

	//////*****************************/////

	old_pop_ptr = &(oldpop);
	ranking(old_pop_ptr);			//Find ranks for initial population
	cout << "Find Pareto for Initial Population: COMPLETED" << std::endl;

	//////*****************************/////

	//printf("**** REPORTS INITIAL POPULATION ****\n");

	//for (int i = 0; i < popSize; i++)		
	//{
	//	printf("%d- RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d - Crowd-Dist:%f - %d.RankSize:%d\n", i + 1, old_pop_ptr->ind[i].numRS, old_pop_ptr->ind[i].numDC,
	//		old_pop_ptr->ind[i].numFac, old_pop_ptr->ind[i].fitness[0], old_pop_ptr->ind[i].fitness[1], old_pop_ptr->ind[i].rank,old_pop_ptr->ind[i].cub_len,
	//		i + 1, old_pop_ptr->rankno[i]);

	//}
	//printf("Maximum Rank in Initial Population:%d\n", old_pop_ptr->maxrank);

	int maxrank1;
	fprintf(writeResult, "Gen\t Sol\t Cost\t Cov\t FAC\t DC\t RS\t XCoord\t YCoord\t CostTime\t FSelectionTime\t TotalTime\t MutationTime\n");

	for (int i = 0; i < popSize; i++)
	{
			for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
			{
				fprintf(writeResult, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %f\t %f\t %f\t %f\t %f\t %f\n", 0, i+1, old_pop_ptr->ind[i].fitness[0] * -1, old_pop_ptr->ind[i].fitness[1],
					old_pop_ptr->ind[i].facilitySet.size(), old_pop_ptr->ind[i].numDC, old_pop_ptr->ind[i].numRS,
					old_pop_ptr->ind[i].facilitySet[j].CoordX, old_pop_ptr->ind[i].facilitySet[j].CoordY,
					costTime, fselectiontime, totaltime, mutationtime);
			
			}
	} 

	 /********************************************************************/			
    /*----------------------GENERATION STARTS HERE----------------------*/			
   /********************************************************************/			


	for (int g = 0; g < ngen; g++)
	{
		cout << "GENERATION:::: " <<g+1<<" ::::: STARTS"<< std::endl;

		old_pop_ptr = &(oldpop);		
		mate_pop_ptr = &(matepop);
		
		g_counter += 1;

		//////*****************************/////

		for (int i = 0; i < popSize; i++)
		{
			for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
			{
				find_fcov(old_pop_ptr->ind[i].facilitySet[j]);
			}
		}

		nselect(old_pop_ptr, mate_pop_ptr);	//Selection for constructing mating pool
		
		cout << "Selection for Mating Pool: COMPLETED" << std::endl;

		new_pop_ptr = &(newpop);
		mate_pop_ptr = &(matepop);

		//////*****************************/////
		
		//////////----------- Efficiency of Coverage -----------------//////

		if (g == 0)
		{
		//	for (int i = 0; i < popSize; i++)
		//	{
		//		for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
		//		{
		//			find_fcov(old_pop_ptr->ind[i].facilitySet[j]);
		//		}
		//	}

			fprintf(writeEfficiency, "mean_encov\t max_encov\t min_encov\t mean_ecov\t max_ecov\t min_ecov\n ");
		}

		float mean_encov = 0.0;
		float max_encov = 0;
		float min_encov = 99999;
		avg_fac = 0;
		float mean_ecov = 0.0;
		float max_ecov = 0;
		float min_ecov = 99999;

		old_pop_ptr = &(oldpop);
		
		find_metrics(old_pop_ptr, mean_encov, max_encov, min_encov, avg_fac, mean_ecov, max_ecov, min_ecov);		//encov, avg_fac and ecov metrics

		cout << "Find Metrics (Ecov, Encov, Average Facility) for Parent Population: COMPLETED" << std::endl;

		std::cout << "Mean encov_s: " << mean_encov<< std::endl;
		std::cout << "Max encov_s: " << max_encov << std::endl;
		std::cout << "Mean ecov_s: " << mean_ecov << std::endl;
		std::cout << "Max ecov_s: " << max_ecov << std::endl;
		fprintf(writeEfficiency, "%f\t %f\t %f\t %f\t %f\t %f\n", mean_encov, max_encov, min_encov, mean_ecov, max_ecov, min_ecov);

		//if (max_encov > 1)			//Alert For an Error Issue
		//{
		//	for (int i = 0; i < popSize; i++)
		//	{
		//		for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
		//		{
		//			std::cout << i << "." << j << " fcov_s: " << old_pop_ptr->ind[i].facilitySet[j].facCov << "---Cov_s: "
		//				<< old_pop_ptr->ind[i].fitness[1] << "---encov_s: " << encov[i] << std::endl;
		//		}
		//	}
		//	std::cout << "ERROR: encov is greater than 1" << endl;
		//	std::cin >> STOP;
		//}

		std::cout << "Population Mean Facility: " << avg_fac << std::endl;
		
		starting = clock();

		crossover(mate_pop_ptr,new_pop_ptr);	
		
		ending = clock();
		crossTime += ExecTime(starting, ending);

		cout << "Crossover: COMPLETED" << std::endl;

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);		//Find coverage objective

		//find_numFac(new_pop_ptr);
		
		//cout << "Find Number of Facilites of Offspring: COMPLETED" << std::endl;

		/*for (int i = 0; i < popSize; i++)
		{
				std::cout << i << ".(NEW)Facility Size: " << new_pop_ptr->ind[i].facilitySet.size() << std::endl;
		}*/

		/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
		for (int i = 0; i < popSize; i++)
		{
			mate_pop_ptr->ind[i].facilitySet.clear();
		}

		float meanFacility = 0.0;
		int max3 = 0;
		int min3 = 999999;

		for (int i = 0; i < popSize; i++)
		{
			meanFacility += (float)new_pop_ptr->ind[i].facilitySet.size() / (float)popSize;
			if (new_pop_ptr->ind[i].facilitySet.size() > max3)
			{
				max3 = new_pop_ptr->ind[i].facilitySet.size();
			}
			if (new_pop_ptr->ind[i].facilitySet.size() < min3)
			{
				min3 = new_pop_ptr->ind[i].facilitySet.size();
			}
			//float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

			//fprintf(writer, "%f\n", ratio);
		}

		//*************** Mutation ******************
		m1Count = 0;
		m2CountS1 = 0;
		m2CountS2 = 0;
		m3Count = 0;

		for (int i = 0; i < popSize; i++)
		{
			individual* temp_ptr = &(new_pop_ptr->ind[i]);

			float rnd = dist(rng);
			
			//cout << i << ". pm_s: " << (1 - ecov[i] * encov[i]) << " Random: " << rnd << " ecov: " << ecov[i] << " encov: " << encov[i] << endl;

			if (rnd < (1 - ecov[i] * encov[i]))
			{
				starting = clock();
				//std::cout << "ecov: " << ecov[i] << " encov: " << encov[i] << " avg_fac: "
				//<< avg_fac << " facility size: " << temp_ptr->facilitySet.size() << std::endl;

				if (ecov[i] > tcov && encov[i] <= tncov && temp_ptr->facilitySet.size() > avg_fac)
				{
					//M1_mutation(temp_ptr);	//DELETE FACILITY

					//std::cout << "M1--MUTATED!!: " << i << std::endl;
					m1Count += 1;
				}
				if (ecov[i] > tcov && encov[i] <= tncov && temp_ptr->facilitySet.size() <= avg_fac)
				{
					M2_mutation(temp_ptr);	//RELOCATE FACILITY

					//std::cout << "M2--MUTATED!!: " << i << std::endl;
				}
				if (ecov[i]<tcov && encov[i] > tncov)
				{
					//M3_mutation(temp_ptr);	//ADD FACILITY

					//std::cout << "M3--MUTATED!!: " << i << std::endl;
					m3Count += 1;
				}
				if (ecov[i] <= tcov && encov[i] <= tncov)
				{
					M2_mutation(temp_ptr);	//RELOCATE FACILITY

					//std::cout << "M2--MUTATED!!: " << i << std::endl;
				}

				ending = clock();
				mutationtime += ExecTime(starting, ending);
			}
		}

		new_pop_ptr = &(newpop);

		findCost(new_pop_ptr);			//Find cost objective

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);		//Find coverage objective

		cout << "Find Cost Objective of Offspring Population: COMPLETED" << std::endl;

		cout << "Find Coverage Objective of Offspring Population: COMPLETED" << std::endl;
		
		//////////////////////////////////////

		//find_numFac(new_pop_ptr);

		//for (int i = 0; i < popSize; i++)
		//{
		//	printf("%d-(NEW) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Crowd-Dist:%f\n", i + 1, new_pop_ptr->ind[i].numRS, new_pop_ptr->ind[i].numDC,
		//		new_pop_ptr->ind[i].numFac, new_pop_ptr->ind[i].fitness[0], new_pop_ptr->ind[i].fitness[1], new_pop_ptr->ind[i].cub_len);
		//}
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
			starting = clock();
			/*Elitism And Sharing Implemented*/
			keepalive(old_pop_ptr, new_pop_ptr, last_pop_ptr, g + 1); 
			cout << "Non-Dominated Sorting: COMPLETED" << std::endl;
			
			ending = clock();
			nondominatedsortTime += ExecTime(starting, ending);
		/*	for (int i = 0; i < popSize; i++)
			{
				printf("%d-(LAST) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Crowd-Dist:%f\n", i + 1, last_pop_ptr->ind[i].numRS, last_pop_ptr->ind[i].numDC,
					last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].fitness[0], last_pop_ptr->ind[i].fitness[1], last_pop_ptr->ind[i].cub_len);
			}

			if (generation==100)
			{
				std::cin >> STOP;

			}*/

			last_pop_ptr = &(lastpop);
			float	meanFacility2 = 0;
			int max2 = 0;
			int min2 = 999999;
		
			for (int i = 0; i < popSize; i++)
			{
				meanFacility2 += (float)last_pop_ptr->ind[i].facilitySet.size() / (float)popSize;
				if (last_pop_ptr->ind[i].facilitySet.size() > max2)
				{
					max2 = last_pop_ptr->ind[i].facilitySet.size();
				}
				if (last_pop_ptr->ind[i].facilitySet.size() < min2)
				{
					min2 = last_pop_ptr->ind[i].facilitySet.size();
				}
				//float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

				//fprintf(writer, "%f\n", ratio);
			}
			if (g == 0)
			{
				fprintf(writeMutationCount, "M1\t M2S1\t M2S2\t M3\n");

			}
			fprintf(writeMutationCount, "%d\t %d\t %d\t %d\n", m1Count, m2CountS1, m2CountS2, m3Count);

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

			last_pop_ptr = &(lastpop);

			if (g == ngen - 1)
			{
				endingAll = clock();
				totaltime = ExecTime(startingAll, endingAll);
			}

			//Print Last Generation Solutions

			if (g % numSnap == 0 || g == ngen-1)
			{

				int count = 0;
				for (int i = 0; i < popSize; i++)
				{
					if (last_pop_ptr->ind[i].rank==1)
					{
						count += 1;
						/*printf("%d-(LAST) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d- %d.RankSize:%d\n", count, last_pop_ptr->ind[i].numRS, last_pop_ptr->ind[i].numDC,
							last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].fitness[0], last_pop_ptr->ind[i].fitness[1], last_pop_ptr->ind[i].rank,
							i + 1, last_pop_ptr->rankno[i]);	*/					

						for (int j = 0; j < last_pop_ptr->ind[i].facilitySet.size(); j++)
						{
							fprintf(writeResult, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %f\t %f\t %f\t %f\t %f\t %f\n", g + 1, count, last_pop_ptr->ind[i].fitness[0]*-1, last_pop_ptr->ind[i].fitness[1],
								last_pop_ptr->ind[i].facilitySet.size(), last_pop_ptr->ind[i].numDC, last_pop_ptr->ind[i].numRS,
								last_pop_ptr->ind[i].facilitySet[j].CoordX, last_pop_ptr->ind[i].facilitySet[j].CoordY,
								costTime, fselectiontime, totaltime, mutationtime);
						}
					}

				} //Ranksize'ı sürekli üzerine yazdığım için hatalı gözüken sonuçlar verebilir. Kümülatif toplamları popSize'a eşit olana kadar doğru veriyor.
				printf("******************************************\n");
			}

		//	/*==================================================================*/

			/*=======Copying the New Population to Old Population======*/

			old_pop_ptr = &(oldpop);
			last_pop_ptr = &(lastpop);

			for (int j = 0; j < popSize; j++)
			{
				old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[j]);
				last_pop_ptr->ind_ptr = &(last_pop_ptr->ind[j]);
			
				/*Copy faciltySet for each individual*/

				old_pop_ptr->ind_ptr->facilitySet = last_pop_ptr->ind_ptr->facilitySet;

				old_pop_ptr->ind_ptr->numDC = last_pop_ptr->ind_ptr->numDC;

				old_pop_ptr->ind_ptr->numRS = last_pop_ptr->ind_ptr->numRS;

				old_pop_ptr->ind_ptr->numFac = last_pop_ptr->ind_ptr->facilitySet.size();

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
				old_pop_ptr->ind_ptr->avg_dis = last_pop_ptr->ind_ptr->avg_dis;

			}   // end of j

			old_pop_ptr->maxrank = last_pop_ptr->maxrank;
			maxrank1 = last_pop_ptr->maxrank;
			
			cout << "Copy New Generation: COMPLETED" << std::endl;

			/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
			for (int i = 0; i < popSize; i++)
			{
				last_pop_ptr->ind[i].facilitySet.clear();
				last_pop_ptr->ind[i].facilitySet.clear();				
			}

			printf("END-OF GENERATION: %d\n", g + 1);
	} // for the last generation

	 // /*                   Generation Loop Ends                                */
	 // /************************************************************************/
	
	std::cout << "M1: " << m1Count << " M2S1: " << m2CountS1 << " M2S2: " << m2CountS2 << " M3: " << m3Count << std::endl;
	std::cout << "END" << endl;
	std::cout << "Total Distance Calculation= " << findDistTimer << std::endl;
	std::cout << "Total Time = " << totaltime << std::endl;
	std::cout << "Total Cost Time = " << costTime << std::endl;
	std::cout << "Total InitialGen Time = " << initTime << std::endl;
	std::cout << "Total Crossover Time = " << crossTime << std::endl;
	std::cout << "Total Coverage Time = " << covTime << std::endl;
	std::cout << "Total Mutation Time = " << mutationtime << std::endl;
	std::cout << "Total Non-Dominated Sorting Time = " << nondominatedsortTime << std::endl;

	std::fclose(writeEfficiency);
	std::fclose(writeResult);
	std::fclose(writeMutationCount);

	if (Plot)
	{
		RPath = string("C:\\\"Program Files\"\\R\\R-3.6.2\\bin\\R.exe < C:\\\"Users\"\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\MIP_Sol1_Sol2_Compare.R --no-save");
		system(RPath.c_str());
	}
	cin >> STOP;
	return 0;

}