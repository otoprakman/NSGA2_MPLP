

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_real_distribution<> dist(0.0, 1.0);

float ExecTime(float start, float stop);		// Print Execution Time of a function

facility affine_comb(facility a, facility b);				// Affine Combination of two facilities- generate only one facility

float findDistance(float a1, float a2, float b1, float b2); // Find Eucledean Distances

void findCoverage(population* pop_ptr);						// Find Coverage of a Facility

void findCost(population* pop_ptr);						// Find number of DC-RS and Cost value

bool compare(facility a, facility b); 

int indcmp(int* ptr1, int* ptr2);							/*Routine comparing the two individuals in terms of cost and coverage objectives*/

void ranking(population* pop_ptr);							// Ranking individuals of population to create fronts

void find_numFac(population* pop_ptr);						//find number of facilities of individuals in a population

void findCrowding(population* pop_ptr, int rnk);			//Find crowding distances after ranking individuals

void sort(int numInd);										/*Sort the arrays in ascending order of the fitness*/

void move_points(facility &a, facility b, float distance);

void addFacility(individual& ind, facility fac1, facility fac2);

void find_fncov(std::vector<facility> &a);

void find_metrics(population* pop_ptr, float& mean_encov, float& max_encov, float& min_encov, float& avg_fac, 
	float& mean_ecov, float& max_ecov, float& min_ecov);

//##########################################################################
//#########################  FUNCTIONS  #################################//
//##########################################################################


float ExecTime(float start, float stop)
{
	float time_taken = float(stop - start) / float(CLOCKS_PER_SEC);
	return time_taken;
}

void find_metrics(population* pop_ptr, float &mean_encov, float &max_encov, float &min_encov, float &avg_fac, 
	float& mean_ecov, float& max_ecov, float& min_ecov) {

	int total_fcov = 0;

	for (int i = 0; i < popSize; i++)
	{
		ecov[i] = 0;
		encov[i] = 0;
		total_fcov = 0;

		for (int j = 0; j < pop_ptr->ind[i].facilitySet.size(); j++)
		{
			total_fcov += pop_ptr->ind[i].facilitySet[j].facCov;
		}

		encov[i] = 1.00 * pop_ptr->ind[i].fitness[1] / (float)total_fcov;

		ecov[i] = 1.00 * pop_ptr->ind[i].fitness[1] / (float)numDemand;

		mean_encov += encov[i] / (float)popSize;
		mean_ecov += ecov[i] / (float)popSize;

		//std::cout << i << ".Ecov: " << ecov[i] << " Encov: " << encov[i] << " mean_encov: " << mean_encov <<" NumFac: "<<pop_ptr->ind[i].facilitySet.size()<<std::endl;

		if (encov[i] > max_encov)
		{
			max_encov = encov[i];
		}
		if (encov[i] < min_encov)
		{
			min_encov = encov[i];
		}
		if (ecov[i] > max_ecov)
		{
			max_ecov = ecov[i];
		}
		if (ecov[i] < min_ecov)
		{
			min_ecov = ecov[i];
		}
	}

	for (int i = 0; i < popSize; i++)
	{
		avg_fac += 1.00 * old_pop_ptr->ind[i].facilitySet.size() / (float)popSize;
		/*std::cout << i << ".Facility Size: " << old_pop_ptr->ind[i].facilitySet.size() << std::endl;*/
	}
}

void find_fcov(facility &a)
{
	int coverage = 0;

	for (int i = 0; i < numDemand; i++)
	{
		if (findDistance(demandSet.CoordX[i],
			demandSet.CoordY[i],
			a.CoordX,
			a.CoordY) <= rd)
		{
			coverage += demandSet.Value[i];
		}
	}
	a.facCov = coverage;
}

void find_fncov(std::vector<facility> &a) {

	for (int i = 0; i < numDemand; i++)
	{
		for (int j = 0; j < a.size(); j++)
		{
			if (findDistance(demandSet.CoordX[i],
				demandSet.CoordY[i],
				a[j].CoordX,
				a[j].CoordY) <= rd)
			{
				a[j].nfacCov += 1;
				break;
			}
		}
	}
}

float findDistance(float a1, float a2, float b1, float b2)
{	/*Find Euclidean Distances Between two Locations*/
	return sqrt(pow(a1 - b1, 2) + pow(a2 - b2, 2) * 1.0);
}

void findCoverage(population* pop_ptr) {
	
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
	
	int temp_numFac,
		coverage;

	for (int i = 0; i < popSize; i++)
	{
		temp_numFac = pop_ptr->ind_ptr->facilitySet.size();
		coverage = 0;

		for (int j = 0; j < numDemand; j++)
		{
			for (int k = 0; k < temp_numFac; k++)
			{
				
				if (findDistance(demandSet.CoordX[j],
					demandSet.CoordY[j],
					pop_ptr->ind_ptr->facilitySet[k].CoordX,
					pop_ptr->ind_ptr->facilitySet[k].CoordY) <= rd)
				{
					coverage = coverage + demandSet.Value[j];
					break;
				}
			}
		}
		
		pop_ptr->ind_ptr->fitness[1] = coverage;
		pop_ptr->ind_ptr = &(pop_ptr->ind[i+1]);
	}

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
}


void move_points(facility &a, facility b, float distance)	//Move a at the direction of b --> a
{
	float vec_x;
	float vec_y;
	vec_x = a.CoordX - b.CoordX;
	vec_y = a.CoordY - b.CoordY;

	vec_x /= distance;
	vec_y /= distance;
	
	a.CoordX = a.CoordX - (rc - distance) * vec_x;
	a.CoordY = a.CoordY - (rc - distance) * vec_y;

}

facility affine_comb(facility a, facility b) 
{
	facility c;
	std::random_device random_device;
	std::mt19937 random_engine(random_device());
	std::normal_distribution<float> distribution(mu, sigma);

	if (a.CoordX == b.CoordX && a.CoordY == b.CoordY)
	{
		c.CoordX = a.CoordX;
		c.CoordY = a.CoordY;
	}
	else
	{
		do
		{
			float beta = distribution(random_engine);

			c.CoordX = a.CoordX * beta + b.CoordX * (1 - beta);
			c.CoordY = a.CoordY * beta + b.CoordY * (1 - beta);
			/*if (c.CoordX < 0.0 || c.CoordY < 0.0 || c.CoordX > 10.0 || c.CoordY > 10.0) printf("(Beta:%f)Minus location Detected X:%f - Y:%f X:%f - Y:%f!!\n", beta, a.CoordX, a.CoordY, b.CoordX, b.CoordY);
			printf("Beta:%f\n", beta);*/
		} while (c.CoordX < minLoc || c.CoordY < minLoc || c.CoordX > maxLoc || c.CoordY > maxLoc);

	}

	/*printf("RETURNED X:%f Y:%f\n", c.CoordX, c.CoordY);*/

	return c;
}



bool compare(facility a, facility b)
{
	//for descending order replace with a.roll >b.roll
	if (a.facCov > b.facCov)
		return 1;
	else
		return 0;
}

bool find_min(const facility& lhs, const facility& rhs)
{
	return lhs.nfacCov < rhs.nfacCov;
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
		pop_ptr->ind_ptr->numFac = pop_ptr->ind_ptr->facilitySet.size();

	}
	
}

float fpara1[popSize][2];		//fitness values of individuals in the same rank

void findCrowding(population* pop_ptr, int rnk)
{

	float length[popSize][2],		//distances
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






