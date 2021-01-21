#pragma once

void facility_selection(std::vector<facility> a, int size, individual &offspring);

void combine_ind(individual* a, individual* b, individual &offspring_fac); // Generate new offspring from two individuals

void crossover(matepopulation* pop_ptr, population* new_pop_ptr); // Create new population from matepopulation

void crossover(matepopulation* matepop_ptr, population* new_pop_ptr)
{
	int k = 0;

	for (int i = 0; i < 2 * popSize; i += 2)
	{
		individual* temp_ptr = &(matepop_ptr->ind[i]);
		matepop_ptr->ind_ptr = &(matepop_ptr->ind[i + 1]);
		new_pop_ptr->ind_ptr = &(new_pop_ptr->ind[k]);

		//std::cout << "Facility Size: a: " << matepop_ptr->ind_ptr->facilitySet.size() << " b: " << temp_ptr->facilitySet.size() << std::endl;

		combine_ind(matepop_ptr->ind_ptr, temp_ptr, new_pop_ptr->ind[k]); //w.o cartesian filter

		if (new_pop_ptr->ind[k].facilitySet.empty())
		{
			printf("ZERO FACILITY \n");
			cin >> k;
		}

		k += 1;
	}

}

void combine_ind(individual* a, individual* b, individual &offspring)
{
	bool add = 1;
	std::vector<facility> temp_facilitySet;

	for (int i = 0; i < a->facilitySet.size(); i++)
	{
		temp_facilitySet.push_back(a->facilitySet[i]);

		add = 1;

		for (int j = 0; j < b->facilitySet.size(); j++)
		{
			temp_facilitySet.push_back(affine_comb(a->facilitySet[i], b->facilitySet[j], a->avg_dis, b->avg_dis));
			
			if (add) {
				temp_facilitySet.push_back(b->facilitySet[j]);
				add = 0;
			}
		}
	}

	//cout << "CROSSOVER::Find Cartesian Product of Individuals: COMPLETED" << std::endl;

	facility_selection(temp_facilitySet, temp_facilitySet.size(), offspring);
}

void facility_selection(std::vector<facility> a, int size, individual &offspring) {

	int max = 0;
	int min = 999999999;

	float temp_prob;

	starting = clock();

	//for (int i = 0; i < size; i++)
	//{
	//	find_fcov(a[i]);
	//}

	//cout << "CROSSOVER::Find fcov of Offspring Candidate Facilities: COMPLETED" << std::endl;

	//std::sort(a.begin(), a.end(), compare);		//Sort in descending order of facility coverages

	//std::random_shuffle(a.begin(), a.end());

	//cout << "CROSSOVER::Sort fcov of Offspring Candidate Facilities: COMPLETED" << std::endl;

	for (int i = 0; i < size; i++)
	{
		a[i].nfacCov = 0;
	}

	find_fncov(a);

	//cout << "CROSSOVER::Find fncov of Offspring Candidate Facilities: COMPLETED" << std::endl;

	for (int i = 0; i < size; i++)
	{
		if (max < a[i].nfacCov)
		{
			max = a[i].nfacCov;
		}
		if (min > a[i].nfacCov)
		{
			min = a[i].nfacCov;
		}
	}

//	std::cout << "Max:" << max << " Min:" << min << " AvgFac:" << avg_fac << std::endl;

	double coef = 0.0;

	if (dynamic == 1)
	{
		coef = (double)g_counter / (double)ngen;
	}
	if (adaptive == 1)
	{
		coef = (avg_fac) / (float)maxInitFacility;
	}
	if (normal == 1)
	{
		coef = 1.00;
	}

	if (max == min)
	{
		for (int i = 0; i < size; i++)
		{
			temp_prob = 1.0;

			float rnd = dist(rng);

			if ((rnd * coef) <= temp_prob)
			{
				offspring.facilitySet.push_back(a[i]);
			}
		}

	}
	else
	{
		for (int j = 0; j < size; j++)
		{
			temp_prob = 1.00 * (a[j].nfacCov - min) / ((float)(max - min) + smallest);

			float rnd = dist(rng);
			//std::cout << "Rand:" << rnd << " Prob:" << " Coef:" << coef << temp_prob << std::endl;
			if ((rnd * coef) <= temp_prob)
			{
				offspring.facilitySet.push_back(a[j]);
				//printf("Prob: %f--Random: %f--Cov: %d--Index:%d\n", temp_prob, rnd, a[j].nfacCov, j);
			}
		}
	}

	//cout << "CROSSOVER::Filter Offspring Candidate Facilities: COMPLETED" << std::endl;

	ending = clock();
	fselectiontime += ExecTime(starting, ending);

}

