#pragma once
#include "Parameters.h";
#include "MinimumSpanningTree.h";

float findDistance(float a1, float a2, float b1, float b2)
{	/*Find Euclidean Distances Between two Locations*/
	return sqrt(pow(a1 - b1, 2) + pow(a2 - b2, 2) * 1.0);
}

void findCoverage(population* pop_ptr) {
	
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
	demand_ptr = &(demandSet);

	int temp_numFac;

	for (int i = 0; i < popSize; i++)
	{
		temp_numFac = pop_ptr->ind_ptr->numFac;
		int coverage = 0;

		for (int j = 0; j < numDemand; j++)
		{
			for (int k = 0; k < temp_numFac; k++)
			{
				pop_ptr->ind_ptr->facility_ptr = &(pop_ptr->ind_ptr->facilitySet[k]);
				
				if (findDistance(demand_ptr->CoordX[j],
					demand_ptr->CoordY[j],
					pop_ptr->ind_ptr->facility_ptr->CoordX,
					pop_ptr->ind_ptr->facility_ptr->CoordY) < fd)
				{
					coverage = coverage + demand_ptr->Value[j];
					break;
				}
			}
		}
		
		pop_ptr->ind_ptr->fitness[1] = coverage;
		pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]);
	}
}


void find_numDC(population* pop_ptr) {
	
	
	for (int i = 0; i < popSize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		int temp_numFac = pop_ptr->ind_ptr->numFac;

		Graph mst(temp_numFac);
		
		for (int j = 0; j < temp_numFac; j++)
		{
			facility* temp_facility = &(pop_ptr->ind_ptr->facilitySet[j]);

			for (int k = 0; k < temp_numFac; k++)
			{
				pop_ptr->ind_ptr->facility_ptr = &(pop_ptr->ind_ptr->facilitySet[k]);
				mst.AddEdge(j, k, findDistance(
					temp_facility->CoordX,
					temp_facility->CoordY,
					pop_ptr->ind_ptr->facility_ptr->CoordX,
					pop_ptr->ind_ptr->facility_ptr->CoordX));
			}
		}

		mst.kruskal_algorithm();
		
		pop_ptr->ind_ptr->numDC = mst.returnVars();
		pop_ptr->ind_ptr->numRS = pop_ptr->ind_ptr->numFac - pop_ptr->ind_ptr->numDC;
		pop_ptr->ind_ptr->fitness[0] = (pop_ptr->ind_ptr->numRS) * costRS + (pop_ptr->ind_ptr->numDC) * costDC;
		
	}	
}


void select(population* old_pop_ptr, population* pop2_ptr)		//OLD-DATED SELECTION PROCEDURE
{
	pop2_ptr->ind_ptr = &(pop2_ptr->ind[0]);
	
	int q = 0;

	for (int i = 0; i < popSize; i++)
	{
		old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[i]);
		
		if (old_pop_ptr->ind_ptr->rank==1)
		{
			*pop2_ptr->ind_ptr = *old_pop_ptr->ind_ptr;
			for (int j = 0; j < old_pop_ptr->ind_ptr->numFac; j++)
			{
				pop2_ptr->ind_ptr->facilitySet[j].CoordX = old_pop_ptr->ind_ptr->facilitySet[j].CoordX;
			}
			q += 1;
			pop2_ptr->ind_ptr = &(pop2_ptr->ind[q]);
		}
	}
}

int findCoverage_facility(facility a)
{
	int coverage = 0;

	for (int i = 0; i < numDemand; i++)
	{
		demand_ptr = &(demandSet);
		if (findDistance(demand_ptr->CoordX[i],
			demand_ptr->CoordY[i],
			a.CoordX,
			a.CoordY) < fd)
		{
			coverage += demand_ptr->Value[i];
		}
	}
	return coverage;
}

facility affine_comb(facility* a, facility* b) 
{
	facility c;

	std::default_random_engine generator;
	std::normal_distribution<float> distribution(mu, sigma);

	float beta = distribution(generator);

	c.CoordX = a->CoordX * beta + b->CoordX * (1 - beta);
	c.CoordY = a->CoordY * beta + b->CoordY * (1 - beta);
	return c;
}

facility* offspring_facility(facility a[], int size)   // Create offspring using cartesian product of two individuals
{
	float temp_probSet[max_numFac];
	int temp_covSet[max_numFac];		//Change sie of array, it should be greater than max_numFac (size > max_numFac)
	facility b[max_numFac];
	int k = 0;

	for (int i = 0; i < size; i++)
	{
		temp_covSet[i] = findCoverage_facility(a[i]);
		if (temp_covSet[i]==0)
		{
			printf("Zero Coverage detected and assigned to 1\n");
			temp_covSet[i] = 1;
		}
	}

	int min = *std::min_element(temp_covSet, temp_covSet + size);
	int max = *std::max_element(temp_covSet, temp_covSet + size);

	for (int j = 0; j < size; j++)
	{
		temp_probSet[j] = 1.00*(temp_covSet[j] - min) / (max - min);
		
		float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

		if (rnd<= temp_probSet[j])
		{
			b[k] = a[j];
			k += 1;			
		}
	}
	for (int i = 0; i < max_numFac; i++)
	{
		
	}
	return b;
}


individual combine_ind(individual* a, individual* b, individual* offspring_fac)
{
	facility temp_facilitySet[max_numFac];
	int k = 0;

	for (int i = 0; i < a->numFac; i++)
	{
		a->facility_ptr = &(a->facilitySet[i]);

		for (int j = i; j < b->numFac; j++)
		{
			b->facility_ptr = &(b->facilitySet[j]);
			temp_facilitySet[k] = affine_comb(a->facility_ptr, b->facility_ptr);
			k += 1;
		}
	}

	if (k != 1)
	{
		offspring_fac->facility_ptr = offspring_facility(temp_facilitySet, k);

		for (int i = 0; i < max_numFac; i++)
		{
			offspring_fac->facilitySet[i] = *offspring_fac->facility_ptr;
			offspring_fac->facility_ptr += 1;
		}
		
	}
	return *offspring_fac;
}

void crossover(population* pop_ptr, population* new_pop_ptr) 
{
	int k = 0;
	
	for (int i = 0; i < popSize; i++)			// UNN. # of LOOP!!!!!
	{
		for (int j = 0; j < popSize; j++)		// UNN. # of LOOP!!!!!
		{

			if (pop_ptr->ind[i].rank == 1 && pop_ptr->ind[j].rank == 1 && i!=j) 
			{
				individual* temp_ptr = &(pop_ptr->ind[j]);
				pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
				new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[k]);

				new_pop_ptr->ind[k] = combine_ind(pop_ptr->ind_ptr, temp_ptr, new_pop_ptr->ind_ptr); 
				k += 1;
			}
		}	
	}
	
}

/*Routine Comparing the two individuals*/

int indcmp(int* ptr1, int* ptr2)
{
	float fit1[maxfun], fit2[maxfun];
	int value, m, n;
	for (int i = 0; i < maxfun; i++)
	{
		fit1[i] = *ptr1++;			//fit[0]:Cost, fit[1]:Coverage
		fit2[i] = *ptr2++;
	}

	if ((fit1[1] >= fit2[1] && fit1[0] < fit2[0]) || (fit1[1] > fit2[1] && fit1[0] <= fit2[0])) 
	{
		value = 1;
	}
	else if ((fit1[1] <= fit2[1] && fit1[0] > fit2[0]) || (fit1[1] < fit2[1] && fit1[0] >= fit2[0]))
	{
		value = 2;
	}
	else 
	{
		value = 3;
	}
	return value;
}


/*This also demarkates the different Pareto Fronts*/

void ranking(population* pop_ptr)
{
	int j,
		rnk,           /*rank*/
		val,           /*value obtained after comparing two individuals*/
		nondom,        /*no of non dominated members*/
		maxrank1,      /*Max rank of the population*/
		rankarr[maxpop]; /*Array storing the individual number at a rank*/


	int* ptr1, * ptr2;

	/*------------------------------* RANKING *------------------------------*/

	/*Initializing the ranks to zero*/
	rnk = 0;

	nondom = 0;
	maxrank1 = 0;

	/*min_fit is initialize to start distributing the dummy fitness =
	  popsize to the rank one individuals and keeping the record such
	  that the minimum fitness of the better rank individual is always
	  greater than max fitness of the relatively worse rank*/


	  /*Difference in the fitness of minimum dummy fitness of better rank
		and max fitness of the next ranked individuals*/

		/*Initializing all the flags to 2*/

	for (int i = 0; i < popSize; i++)
	{
		pop_ptr->ind[i].flag = 2;
	}

	int q = 0;

	for (int k = 0; k < popSize; k++, q = 0)
	{
		for (j = 0; j < popSize; j++)
		{
			if (pop_ptr->ind[j].flag != 1)break;
			/*Break if all the individuals are assigned a rank*/
		}
		if (j == popSize)break;

		rnk = rnk + 1;

		for (int j = 0; j < popSize; j++)
		{
			if (pop_ptr->ind[j].flag == 0) pop_ptr->ind[j].flag = 2;
			/*Set the flag of dominated individuals to 2*/
		}

		for (int i = 0; i < popSize; i++)
		{
			/*Select an individual which rank to be assigned*/

			pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

			if (pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0)
			{
				ptr1 = &(pop_ptr->ind_ptr->fitness[0]);
				
				for ( j = 0; j < popSize; j++)
				{

					/*Select the other individual which has not got a rank*/
					if (i != j)
					{
						if (pop_ptr->ind[j].flag != 1)
						{
							pop_ptr->ind_ptr = &(pop_ptr->ind[j]);
							ptr2 = &(pop_ptr->ind_ptr->fitness[0]);
						/*Compare the two individuals for fitness*/
							val = indcmp(ptr1, ptr2);

							/*VAL = 2 for dominated individual which rank to be given*/
							/*VAL = 1 for dominating individual which rank to be given*/

							/*VAL = 3 for non comparable individuals*/

							if (val == 2)
							{
								pop_ptr->ind[i].flag = 0;/* individual 1 is dominated */
								break;
							}

							if (val == 1)
							{
								pop_ptr->ind[j].flag = 0;/* individual 2 is dominated */

							}

							if (val == 3)
							{
								nondom++;/* individual 1 & 2 are non dominated */
								if (pop_ptr->ind[j].flag != 0)
									pop_ptr->ind[j].flag = 3;
							}

						}   /*if loop ends*/
					}       /* i != j loop ends*/
				}           /*loop over j ends*/

				if (j == popSize)
				{
					/*Assign the rank and set the flag*/
					pop_ptr->ind[i].rank = rnk;
					pop_ptr->ind[i].flag = 1;
					rankarr[q] = i;
					q++;
				}
			}       /*Loop over flag check ends*/
		}           /*Loop over i ends */
		pop_ptr->rankno[rnk - 1] = q;
	}
	maxrank1 = rnk;

	

	/* Find Max Rank of the population    */
	for (int i = 0; i < popSize; i++)
	{
		rnk = pop_ptr->ind[i].rank;

		if (rnk > maxrank1)maxrank1 = rnk;

	}

	pop_ptr->maxrank = maxrank1;

}



/*This is the program used to evaluate the value of the function & errors
************************************************************************/

void func(population* pop_ptr)
{
	/*File ptr to the file to store the value of the g for last iteration
	  g is the parameter required for a particular problem
	  Every problem is not required*/

	int* fitcost_ptr;	 /*Pointer to the array of fitness function*/
	
	int* fitcov_ptr;
		
	int i, j, k;
	
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);

	/*Initializing the max rank to zero*/
	pop_ptr->maxrank = 0;
	for (i = 0; i < popSize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

		fitcost_ptr = &(pop_ptr->ind_ptr->fitness[0]);
		fitcov_ptr = &(pop_ptr->ind_ptr->fitness[1]);
	}

	/*---------------------------* RANKING *------------------------------*/

	ranking(pop_ptr);

}

void find_numFac(population* pop_ptr) {
	for (int i = 0; i < popSize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		pop_ptr->ind_ptr->numFac = 0;

		for (int j = 0; j < max_numFac; j++)
		{
			if (pop_ptr->ind_ptr->facilitySet[j].CoordX > 0)
			{
				pop_ptr->ind_ptr->numFac += 1;
			}

		}
	}
}







