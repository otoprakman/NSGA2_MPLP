#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

float randomLocation(float minLoc, float maxLoc);						 // Generate random locations

int randomNumFacility(int maxFacility, int minInitFacility);			 // Generate random number of facilities

void initPopulation(population* pop_ptr);		 // Locate random number of faciilities to random locations for each individual


void externalInitPop(population* pop_ptr) {
	int counter;
	float X;
	float Y;

	std::ifstream inpX("C:\\Users...", std::ifstream::in); //file location of xcoords of external initial population
	std::ifstream inpY("C:\\Users...", std::ifstream::in);//file location of xcoords of external initial population
	std::ifstream inpCount("C:\\Users...", std::ifstream::in);//file location of number of facilities in each individual

	for (int i = 0; i < popSize; i++)
	{
		inpCount >> counter;
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);

		for (int j = 0; j < counter; j++)
		{
			inpX >> X;
			inpY >> Y;
			pop_ptr->ind_ptr->facilitySet.push_back({ X, Y, 0, 0 });
		}

		pop_ptr->ind_ptr->numFac = counter;
	}

	inpX.close();
	inpY.close();
	inpCount.close();
}


void initPopulation(population* pop_ptr)
{
	int temp_numFac;

	for (int i = 0; i < popSize; i++)
	{
		pop_ptr->ind_ptr = &(pop_ptr->ind[i]);
		temp_numFac = randomNumFacility(maxInitFacility, minInitFacility);

		for (int j = 0; j < temp_numFac; j++)
		{
			pop_ptr->ind_ptr->facilitySet.push_back({randomLocation(minLoc, maxLoc), randomLocation(minLoc, maxLoc), 0, 0 });
		}

		pop_ptr->ind_ptr->numFac = temp_numFac;		
	}
}

float randomLocation(float minLoc, float maxLoc) {					//Generate random float numbers for locations
	float number = 0.0;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<> dist(minLoc, maxLoc);

	number = dist(rng);

	return number;
}

int randomNumFacility(int maxFacility, int minInitFacility) {		//Generate random integer numbers for initial facility numbers in Solutions
	int number ;
	
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<mt19937::result_type> dist(minInitFacility, maxInitFacility);
	
	number = dist(rng);
	
	return number;
}