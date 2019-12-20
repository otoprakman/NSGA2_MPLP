#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Parameters.h";
#include "mrand.h";

using namespace std;

float randomLocation(float minLoc, float maxLoc) {					//Generate random float numbers for locations
	float number = 0.0;
	return number = minLoc + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoc - minLoc))); //!!!!Delete int expression!!!!!
}

int randomNumFacility(int maxFacility) {							//Generate random integer numbers for initial facility numbers in Solutions
	int number = 0;
	
	return number= 1 + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (maxFacility - 1)));
}

void initPopulation(population* pop_ptr);

void initPopulation(population* pop_ptr)
{
	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
	pop_ptr->ind_ptr->facility_ptr = &(pop_ptr->ind_ptr->facilitySet[0]);
	int temp_numFac;

	for (int i = 0; i < popSize; i++)
	{
		temp_numFac = randomNumFacility(maxInitFacility);

		for (int j = 0; j < temp_numFac; j++)
		{
			pop_ptr->ind_ptr->facility_ptr->CoordX = randomLocation(minLoc, maxLoc);
			pop_ptr->ind_ptr->facility_ptr->CoordY = randomLocation(minLoc, maxLoc);
			pop_ptr->ind_ptr->facility_ptr = &(pop_ptr->ind_ptr->facilitySet[j + 1]);
		}
		pop_ptr->ind_ptr->numFac = temp_numFac;
		pop_ptr->ind_ptr = &(pop_ptr->ind[i + 1]);
		pop_ptr->ind_ptr->facility_ptr = &(pop_ptr->ind_ptr->facilitySet[0]);
	}

	pop_ptr->ind_ptr = &(pop_ptr->ind[0]);
}

