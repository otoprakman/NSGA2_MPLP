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


int main(int, char**)
{
	startingAll = clock();

	int STOP;

	old_pop_ptr = &(oldpop);
	new_pop_ptr = &(newpop);
	mate_pop_ptr = &(matepop);

	srand(time(NULL));

	readWrite();						//Read Demand attributes from the file

	old_pop_ptr = &(oldpop);
	initPopulation(old_pop_ptr);		//Generate initial population
	
	/////-------Init Population check----------/////

	//for (int i = 0; i < popSize; i++)
	//{
	//	individual* ind_ptr = &(oldpop.ind[i]);

	//	for (int j = 0; j < ind_ptr->facilitySet.size(); j++)
	//	{
	//		std::cout <<i<< ". X: "<<ind_ptr->facilitySet[j].CoordX << "---Y: "<<ind_ptr->facilitySet[j].CoordY << std::endl;
	//	}
	//}
	//std::cin >> STOP;

	/////*************************************//////

	old_pop_ptr = &(oldpop);
	findCoverage(old_pop_ptr);			//Find coverage objective

	////////----- Coverage Check -------////

	for (int i = 0; i < popSize; i++)
	{
		individual* ind_ptr = &(oldpop.ind[i]);
		std::cout << i << ". Coverage: " << ind_ptr->fitness[1] << std::endl;
	}
	//std::cin >> STOP;

	//////*****************************/////

	old_pop_ptr = &(oldpop);
	for (int i = 0; i < popSize; i++)
	{
		old_pop_ptr->ind[i]=find_numDCP(old_pop_ptr->ind[i], i, 0);			//Find cost objective
		/*std::cout << i << ". Cost: " << old_pop_ptr->ind[i].fitness[0] <<"---NumFac: "<< old_pop_ptr->ind[i].facilitySet.size()<< std::endl;*/
	}
	std::cin >> STOP;

	for (int i = 0; i < popSize; i++)
	{

		for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
		{
			old_pop_ptr->ind[i].facilitySet[j].facCov = findCoverage_facility(old_pop_ptr->ind[i].facilitySet[j]);
		}
	}

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


		//////////----------- Efficiency of Coverage -----------------//////
		int total_cov = 0;
		float mean_efficiency = 0.0;
		float sum_cov = 0.0;
		float sum_fcov = 0.0;

		for (int i = 0; i < popSize; i++)
		{
			for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
			{
				old_pop_ptr->ind[i].facilitySet[j].facCov = findCoverage_facility(old_pop_ptr->ind[i].facilitySet[j]);
			}
		}

		if (generation == 0)
		{
			fprintf(writeEfficiency, "mean_Cov\t mean_Sum_fcov\t mean_eff\t max_eff\t min_eff\n ");
		}

		float max = 0;
		float min = 999999;
		
		for (int i = 0; i < popSize; i++)
		{
			
			for (int j = 0; j < old_pop_ptr->ind[i].facilitySet.size(); j++)
			{
				total_cov += old_pop_ptr->ind[i].facilitySet[j].facCov;
				
			}

			sum_cov += (float)old_pop_ptr->ind[i].fitness[1];
			
			eloc[i] = find_eloc_s(old_pop_ptr->ind[i]);

			mean_efficiency += (float)old_pop_ptr->ind[i].fitness[1] / (float)total_cov;
			sum_fcov += (float)total_cov;
			total_cov = 0;

			if (eloc[i] > max)
			{
				max = eloc[i];
			}
			if (eloc[i] < min)
			{
				min = eloc[i];
			}
		}

		cout << "Mean Efficiency: " << mean_efficiency / (float)popSize << endl;
		fprintf(writeEfficiency, "%f\t %f\t %f\t %f\t %f\n", (sum_cov / (float)popSize), (sum_fcov / (float)popSize),
			(mean_efficiency / (float)popSize), max, min);

		mean_facility = 0;
		
		for (int i = 0; i < popSize; i++)
		{
			mean_facility += (float)old_pop_ptr->ind[i].facilitySet.size() / (float)popSize;
		}

		std::cout << "Population Mean Facility: " << mean_facility << std::endl;

		crossover(mate_pop_ptr,new_pop_ptr);	
	
		new_pop_ptr = &(newpop);

		find_numFac(new_pop_ptr);
		
		/////// LOOK FOR AN ALTERNATIVE INITIALIZATION !!!!
		for (int i = 0; i < popSize; i++)
		{
			mate_pop_ptr->ind[i].facilitySet.clear();
		}

		float	meanFacility = 0.0;
		int max3 = 0;
		int min3 = 999999;

		for (int i=0; i < popSize; i++)
		{
			meanFacility += (float)new_pop_ptr->ind[i].numFac/(float)popSize;
			if (new_pop_ptr->ind[i].numFac > max3)
			{
				max3 = new_pop_ptr->ind[i].numFac;
			}
			if (new_pop_ptr->ind[i].numFac < min3)
			{
				min3 = new_pop_ptr->ind[i].numFac;
			}
			//float ratio = ((float)new_pop_ptr->ind[i].numFac) / ((float)mate_pop_ptr->ind[j].numFac * (float)mate_pop_ptr->ind[j + 1].numFac);

			//fprintf(writer, "%f\n", ratio);
		}

		if (generation == 0)
		{
			fprintf(writeBeforeSelection, "maxBefore\t meanBefore\t minBefore\n ");
		}

		fprintf(writeBeforeSelection, "%d\t %f\t %d\n", max3, meanFacility, min3);

		new_pop_ptr = &(newpop);

		for (int i = 0; i < popSize; i++)
		{
			new_pop_ptr->ind[i]=find_numDCP(new_pop_ptr->ind[i], i, 0);			//Find cost objective
		}

		new_pop_ptr = &(newpop);
		findCoverage(new_pop_ptr);			//Find coverage objective

		new_pop_ptr = &(newpop);

		////FIND EFFICIENCY SCORE FOR MUTATION

		for (int i = 0; i < popSize; i++)
		{
			for (int j = 0; j < new_pop_ptr->ind[i].facilitySet.size(); j++)
			{
				new_pop_ptr->ind[i].facilitySet[j].facCov = findCoverage_facility(new_pop_ptr->ind[i].facilitySet[j]);
			}
		}

		for (int i = 0; i < popSize; i++)
		{
			eloc[i] = 0;
			eloc[i] = find_eloc_s(new_pop_ptr->ind[i]);
			//cout << i << ".eloc: " << eloc[i] << endl;
		}

		for (int i = 0; i < popSize; i++)
		{
			individual* temp_ptr = &(new_pop_ptr->ind[i]);

			float rnd = dist(rng);
			if (rnd < (1 - eloc[i]))
			{
				rnd = dist(rng);

				if (rnd < eadj[i])
				{
					M1_mutation(temp_ptr);
					std::cout << "M1--MUTATED!!: " << i << std::endl;
				}
				else
				{
					M2_mutation(new_pop_ptr->ind[i], i);
					std::cout << "M2--MUTATED!!: " << i << std::endl;
				}
			}
			/*else
			{
				continue;
				
				rnd = dist(rng);

				if (rnd < 1 - ((float)temp_ptr->fitness[1] / (float)numDemand))
				{
					
				}
			}*/
		}
		//////////////////////////////////////

		new_pop_ptr = &(newpop);
		find_numFac(new_pop_ptr);

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
			
			/*Elitism And Sharing Implemented*/
			keepalive(old_pop_ptr, new_pop_ptr, last_pop_ptr, generation + 1); 
			last_pop_ptr = &(lastpop);
			float	meanFacility2 = 0;
			int max2 = 0;
			int min2 = 999999;
		
			for (int i = 0; i < popSize; i++)
			{
				meanFacility2 += (float)last_pop_ptr->ind[i].numFac / (float)popSize;
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
			if (generation == 0)
			{
				fprintf(writeAfterSelection, "maxAfter\t meanAfter\t minAfter\n");
			}
			fprintf(writeAfterSelection, "%d\t %f\t %d\n", max2,meanFacility2, min2);

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
			
			//Print Last Generation Solutions

			if (generation % numSnap == 0 || generation == generationNum-1)
			{
				if (generation == 0)
				{
					fprintf(writeResult, "Gen\t Sol\t Cost\t Cov\t FAC\t DC\t RS\t XCoord\t YCoord\t MSTTime\t SortingTime\t TotalTime\n");
				}

				int count = 0;
				for (int i = 0; i < popSize; i++)
				{
					if (last_pop_ptr->ind[i].rank==1)
					{
						count += 1;
						/*printf("%d-(LAST) RS:%d - DC:%d - FAC:%d - Cost:%d - Cov:%d - Rank:%d- %d.RankSize:%d\n", count, last_pop_ptr->ind[i].numRS, last_pop_ptr->ind[i].numDC,
							last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].fitness[0], last_pop_ptr->ind[i].fitness[1], last_pop_ptr->ind[i].rank,
							i + 1, last_pop_ptr->rankno[i]);	*/					

						if (generation == generationNum-1)
						{
							endingAll = clock();
							totaltime = ExecTime(startingAll, endingAll);
						}

						for (int j = 0; j < last_pop_ptr->ind[i].facilitySet.size(); j++)
						{
							fprintf(writeResult, "%d\t %d\t %d\t %d\t %d\t %d\t %d\t %f\t %f\t %f\t %f\t %f\n", generation + 1, count, last_pop_ptr->ind[i].fitness[0]*-1, last_pop_ptr->ind[i].fitness[1],
								last_pop_ptr->ind[i].numFac, last_pop_ptr->ind[i].numDC, last_pop_ptr->ind[i].numRS,
								last_pop_ptr->ind[i].facilitySet[j].CoordX, last_pop_ptr->ind[i].facilitySet[j].CoordY,
								msttime, sortingtime, totaltime);
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

			printf("END-OF GENERATION: %d\n", generation + 1);
	} // for the last generation

	 // /*                   Generation Loop Ends                                */
	 // /************************************************************************/
	
	cout << "END" << endl;
	fclose(writeBeforeSelection);
	fclose(writeAfterSelection);
	fclose(writeEfficiency);
	fclose(writeResult);

	if (Plot)
	{
		RPath = string("C:\\\"Program Files\"\\R\\R-3.6.2\\bin\\R.exe < C:\\\"Users\"\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\MIP_Sol1_Sol2_Compare.R --no-save");
		system(RPath.c_str());
	}
	cin >> STOP;
	return 0;

}