


void printExecTime(float start, float stop, FILE* loc);		// Print Execution Time of a function

float findDistance(float a1, float a2, float b1, float b2); // Find Eucledean Distances

void findCoverage(population* pop_ptr);						// Find Coverage of a Facility

void find_numDC(population* pop_ptr);						// Find number of DC-RS and Cost value

int findCoverage_facility(facility a);						// Find coverage of a single facility using in crossover

facility affine_comb(facility* a, facility* b);				// Affine Combination of two facilities- generate only one facility

facility* offspring_facility(facility a[], int size);		// Create offspring using cartesian product of two individuals

facility* myopic_offspring_facility(facilitySet a, int size);

bool compare(facility a, facility b); 

individual combine_ind(individual* a, individual* b, individual* offspring_fac); // Generate new offspring from two individuals

void crossover(matepopulation* pop_ptr, population* new_pop_ptr, FILE* writer); // Create new population from matepopulation

int indcmp(int* ptr1, int* ptr2);							/*Routine comparing the two individuals in terms of cost and coverage objectives*/

void ranking(population* pop_ptr);							// Ranking individuals of population to create fronts

void find_numFac(population* pop_ptr);						//find number of facilities of individuals in a population

void findCrowding(population* pop_ptr, int rnk);			//Find crowding distances after ranking individuals

void sort(int numInd);										/*Sort the arrays in ascending order of the fitness*/

//##########################################################################
//#########################  FUNCTIONS  #################################//
//##########################################################################


void printExecTime(float start, float stop, FILE* loc)
{
	float time_taken = float(stop - start) / float(CLOCKS_PER_SEC);
	fprintf(loc, "%f\n", time_taken);
}


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
					pop_ptr->ind_ptr->facility_ptr->CoordY) <= fd)
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
	std::random_device random_device;
	std::mt19937 random_engine(random_device());
	std::normal_distribution<float> distribution(mu, sigma);

	do
	{
		float beta = distribution(random_engine);

		c.CoordX = a->CoordX * beta + b->CoordX * (1 - beta);
		c.CoordY = a->CoordY * beta + b->CoordY * (1 - beta);
		/*if (c.CoordX < 0.0 || c.CoordY < 0.0) printf("(Beta:%f)Minus location Detected X:%f - Y:%f !!\n", beta, c.CoordX, c.CoordY);
		printf("Beta:%f\n", beta);*/
	} while (c.CoordX < minLoc || c.CoordY < minLoc || c.CoordX > maxLoc || c.CoordY > maxLoc);


	/*printf("RETURNED X:%f Y:%f\n", c.CoordX, c.CoordY);*/

	return c;
}

facility* myopic_offspring_facility(facilitySet a, int size) {
	
	demand_ptr = &(demandSet);

	int max = 0;
	int min = 999999999;

	float temp_probSet[max_numFac];
	
	facility b[max_numFac];

	int k = 0;
	for (int i = 0; i < size; i++)
	{
		a.facilitySet[i].facCov = findCoverage_facility(a.facilitySet[i]);
	}

	sort(a.facilitySet, a.facilitySet + size, compare);		//Sort in descending order of facility coverages

	for (int i = 0; i < size; i++)
	{
		a.facilitySet[i].facCov = 0;
	}

	for (int i = 0; i < numDemand; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (findDistance(demand_ptr->CoordX[i],
				demand_ptr->CoordY[i],
				a.facilitySet[j].CoordX,
				a.facilitySet[j].CoordY) <= fd)
			{
				a.facilitySet[j].facCov += 1;
				break;
			}
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (max < a.facilitySet[i].facCov)
		{
			max = a.facilitySet[i].facCov;
		}
		if (min > a.facilitySet[i].facCov)
		{
			min = a.facilitySet[i].facCov;
		}
	}

	//printf("Max:%d Min:%d\n", max, min);
	
	if (max == min)
	{
		for (int i = 0; i < size; i++)
		{
			temp_probSet[i] = 1.0;

			float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

			if (rnd <= temp_probSet[i])
			{
				b[k] = a.facilitySet[i];

				//printf("Prob: %f--Selected: %f\n", temp_probSet[i],b[k].CoordX);
				k += 1;
			}
			//printf("Prob: %f\n", temp_probSet[i]);
			//if (i == size - 1)
			//{
			//	printf("******\n");
			//}
		}

	}
	else
	{
		for (int j = 0; j < size; j++)
		{
			temp_probSet[j] = 1.00 * (a.facilitySet[j].facCov - min) / (max - min);

			float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

			if (rnd <= temp_probSet[j])
			{
				b[k] = a.facilitySet[j];
				//printf("Prob: %f--Selected: %f\n", temp_probSet[j],b[k].CoordX);
				k += 1;
			}
		}
	}


	return b;

}


facility* offspring_facility(facilitySet a, int size)   
{
	float temp_probSet[max_numFac];
	int temp_covSet[max_numFac];		//Change size of array, it should be greater than max_numFac (size > max_numFac)
	facility b[max_numFac];
	int k = 0;

	for (int i = 0; i < size; i++)
	{
		temp_covSet[i] = findCoverage_facility(a.facilitySet[i]);
	}

	int min = *std::min_element(temp_covSet, temp_covSet + size);
	int max = *std::max_element(temp_covSet, temp_covSet + size);
	/*printf("Max:%d Min:%d\n", max, min);*/
	if (max==min)
	{
		for (int i = 0; i < size; i++)
		{
			temp_probSet[i] = 1.0;


			float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

			if (rnd <= temp_probSet[i])
			{
				b[k] = a.facilitySet[i];

				//printf("Prob: %f--Selected: %f\n", temp_probSet[j],b[k].CoordX);
				k += 1;
			}
			/*printf("Prob: %f\n", temp_probSet[i]);
			if (i == size - 1)
			{
				printf("******\n");
			}*/
		}

	}
	else
	{
		//float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));  //ALTERNATIVE; if we generate only one random number instead generating for each facilities

		for (int j = 0; j < size; j++)
		{
			temp_probSet[j] = 1.00 * (temp_covSet[j] - min) / (max - min);
			
			float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));

			if (rnd <= temp_probSet[j])
			{
				b[k] = a.facilitySet[j];
				//printf("Prob: %f--Selected: %f\n", temp_probSet[j],b[k].CoordX);
				k += 1;
			}
		}
	}

	return b;
}

bool compare(facility a, facility b)
{
	//for descending order replace with a.roll >b.roll
	if (a.facCov > b.facCov)
		return 1;
	else
		return 0;
}

individual cartesianFilter(individual* a, int size, individual* b, int size2, individual* offspring_fac) {
	
	int total = 0;
	int total2 = 0;
	float cumul = 0.0;
	float cumul2 = 0.0;
	facilitySet return_fac;
	facilitySet return_fac2;
	facilitySet return_facRES;
	facility* ptr1;
	facility* ptr2;
	int k = 0;
	int k2 = 0;
	int m = 0;

	for (int i = 0; i < size; i++)
	{
		a->facilitySet[i].facCov = findCoverage_facility(a->facilitySet[i]);	//find facility coverages of parent 1
		total += a->facilitySet[i].facCov;
	}
	
	for (int i = 0; i < size2; i++)
	{
		b->facilitySet[i].facCov = findCoverage_facility(b->facilitySet[i]); //find facility coverages of parent 2
		total2 += b->facilitySet[i].facCov;
	}

	//for (int i = 0; i < size; i++)
	//{
	//	cout << "A: " << a->facilitySet[i].CoordX <<" Cov: " <<a->facilitySet[i].facCov<< endl;

	//}
	//for (int i = 0; i < size2; i++)
	//{
	//	cout << "B: " << b->facilitySet[i].CoordX << " Cov: " << b->facilitySet[i].facCov << endl;
	//}

	sort(a->facilitySet, a->facilitySet + size, compare);
	sort(b->facilitySet, b->facilitySet + size, compare);

	//std::sort(std::begin(temp_covSet), std::end(temp_covSet), std::greater<>()); //sort in descending order
	//std::sort(std::begin(temp_covSet2), std::end(temp_covSet2), std::greater<>());
	//

	for (int i = 0; i < size; i++)
	{
		if ((cumul - cutoff) < 0.000001 )
		{
			return_fac.facilitySet[k] = a->facilitySet[i];
			k += 1;
			cumul += (1.0 * a->facilitySet[i].facCov) / (1.0 * total);
		}
	}

	
	for (int i = 0; i < size2; i++)
	{
		if ((cumul2 - cutoff) < 0.000001)
		{
			return_fac2.facilitySet[k2] = b->facilitySet[i];
			k2 += 1;
			cumul2 += (1.0 * b->facilitySet[i].facCov) / (1.0 * total2);
		}
	}
	
	/*for (int i = 0; i < k; i++)
	{
		cout << "A+: " << return_fac.facilitySet[i].CoordX << endl;
	}
	for (int i = 0; i < k2; i++)
	{
		cout << "B+: " << return_fac2.facilitySet[i].CoordX << endl;
	}*/


	for (int i = 0; i < k; i++)
	{
		ptr1 = &(return_fac.facilitySet[i]);

		for (int j = 0; j < k2; j++)
		{
			ptr2 = &(return_fac2.facilitySet[j]);

			return_facRES.facilitySet[m] = affine_comb(ptr1, ptr2);
			/*cout << "OFF: " << return_facRES.facilitySet[m].CoordX << endl;*/

			m += 1;
		}
	}

	offspring_fac->facility_ptr = myopic_offspring_facility(return_facRES, m);

	//offspring_fac->facility_ptr = offspring_facility(return_facRES, m);

	for (int i = 0; i <max_numFac; i++) ///////////////////----------WARNING----------///////
	{
		offspring_fac->facilitySet[i] = *offspring_fac->facility_ptr;
		//cout << "OFF+: " << offspring_fac->facilitySet[i].CoordX << endl;

		offspring_fac->facility_ptr++;

	}

	return *offspring_fac;

}

individual combine_ind(individual* a, individual* b, individual* offspring_fac)
{
	facilitySet temp_facilitySet;
	int k = 0;	
	
	for (int i = 0; i < a->numFac; i++)
	{
		a->facility_ptr = &(a->facilitySet[i]);

		for (int j = 0; j < b->numFac; j++)
		{
			b->facility_ptr = &(b->facilitySet[j]);
			temp_facilitySet.facilitySet[k] = affine_comb(a->facility_ptr, b->facility_ptr);
			//if (temp_facilitySet[k].CoordX < 0.0) break;
			k += 1;			
		}

		//if (temp_facilitySet[k-1].CoordX < 0.0) break;
	}


	//offspring_fac->facility_ptr = offspring_facility(temp_facilitySet, k);
	offspring_fac->facility_ptr = myopic_offspring_facility(temp_facilitySet, k);


	for (int i = 0; i < max_numFac; i++)
	{
		offspring_fac->facilitySet[i] = *offspring_fac->facility_ptr;
		offspring_fac->facility_ptr ++;
	}

	//if (k == 1) offspring_fac->facilitySet[0]=temp_facilitySet[0];

	return *offspring_fac;
}

void crossover(matepopulation* matepop_ptr, population* new_pop_ptr,FILE* writer) 
{

	int k = 0;

	for (int i = 0; i < 2*popSize; i+=2)
	{

		individual* temp_ptr = &(matepop_ptr->ind[i]);
		matepop_ptr->ind_ptr = &(matepop_ptr->ind[i+1]);
		new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[k]);
		
		new_pop_ptr->ind[k] = combine_ind(matepop_ptr->ind_ptr, temp_ptr, new_pop_ptr->ind_ptr);

		//new_pop_ptr->ind[k] = cartesianFilter(matepop_ptr->ind_ptr, matepop_ptr->ind_ptr->numFac, temp_ptr, temp_ptr->numFac, new_pop_ptr->ind_ptr);
		
		//for (int i = 0; i < 10; i++)
		//{
		//	cout << "OUT: " << new_pop_ptr->ind[k].facilitySet[i].CoordX << endl;
		//}

		if (new_pop_ptr->ind[k].facilitySet[0].CoordX<0)
		{
			printf("ZERO FACILITY INDEX: %d-- %f\n", k+1, new_pop_ptr->ind[k].facilitySet[0].CoordX);
			cin >> k;
		}

		k += 1;		
	}

}


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
		maxrank1;      /*Max rank of the population*/


	int * ptr1,		//Pointers for objective values
		* ptr2;		

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
			
			/* Meaning of flag;
			   0: Dominated individual
			   1: Assigned a rank 
			   2: Not ranked
			   3: not comparable
			*/
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

			if (pop_ptr->ind_ptr->flag != 1 && pop_ptr->ind_ptr->flag != 0) // select if not dominated and not assigned
			{
				ptr1 = &(pop_ptr->ind_ptr->fitness[0]); //Get cost objective
				
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
					pop_ptr->rankindices[rnk - 1][q] = i;
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

	for (int i = 1; i <= maxrank1; i++)
	{
		findCrowding(pop_ptr, i);
	}
}


void find_numFac(population* pop_ptr) {
	for (int i = 0; i < popSize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		pop_ptr->ind_ptr->numFac = 0;
		for (int j = 0; j < max_numFac; j++)
		{

			if ((pop_ptr->ind_ptr->facilitySet[j].CoordX > 0.0) && (pop_ptr->ind_ptr->facilitySet[j].CoordY > 0.0))
			{
				pop_ptr->ind_ptr->numFac += 1;

			}
		}
	}
	
}

float fpara1[maxpop][2];		//fitness values of individuals in the same rank

void findCrowding(population* pop_ptr, int rnk)
{

	float length[maxpop][2],		//distances
		   max;	
	int i, j, numInd, a;
	float min, Diff;				//Added 18.08.2003

	numInd = pop_ptr->rankno[rnk - 1]; //Array holds number of individuals in the rank

	for (j = 0; j < maxfun; j++)
	{
		for (i = 0; i < numInd; i++)
		{
			fpara1[i][0] = 0;	 //Initializing with 0's
			fpara1[i][1] = 0;
		}

		for (i = 0; i < numInd; i++)
		{
			a = pop_ptr->rankindices[rnk - 1][i];
			fpara1[i][0] = (float)a;				//[i][0/1] 0 is for indices, 1 is for length values
			fpara1[i][1] = pop_ptr->ind[a].fitness[j];
		}

		sort(numInd); /*Sort the arrays in ascending order of the fitness*/

		max = fpara1[numInd - 1][1];
		min = fpara1[0][1];  // Added 18.08.2003
		Diff = max - min;      // Added 18.08.2003 and 5 subsequent lines
		if (Diff < 0.0)
		{
			printf("Something wrong in keepaliven.h\n");
			exit(1);
		}

		for (i = 0; i < numInd; i++)
		{
			if (i == 0 || i == (numInd - 1))
			{
				length[i][0] = fpara1[i][0];
				length[i][1] = 100 * max;
			}
			else
			{
				length[i][0] = fpara1[i][0];
				length[i][1] = fabs(fpara1[i + 1][1] - fpara1[i - 1][1]) / (Diff + smallest); // crowding distances are normalized 18.08.2003
			}
		}
		for (i = 0; i < numInd; i++)
		{
			a = length[i][0];
			pop_ptr->ind[a].cub_len += length[i][1];

		}
	}

	return;
}


void sort(int numInd)
{
	float temp, temp1;
	int i1, j1, k1;
	for (k1 = 0; k1 < numInd - 1; k1++)
	{
		for (i1 = k1 + 1; i1 < numInd; i1++)
		{
			if (fpara1[k1][1] > fpara1[i1][1])
			{
				temp = fpara1[k1][1];
				temp1 = fpara1[k1][0];
				fpara1[k1][1] = fpara1[i1][1];
				fpara1[k1][0] = fpara1[i1][0];
				fpara1[i1][1] = temp;
				fpara1[i1][0] = temp1;
			}
		}
	}
	return;
}






