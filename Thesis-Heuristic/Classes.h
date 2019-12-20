#pragma once





/*
Solution Struct holds;
- Facility set that contains both DC and RS locations
- Coverage Objective
- Cost Objective

class Solution {
private:
public:
	int numFacility;
	int numDC;
	int numRS = numFacility - numDC;
	int Coverage;
	int Cost;
	Facility *facilitySet;

	//METHODS

	Solution();					//Constructor for Creating Pointer
	explicit Solution(int N,Facility arr[]);	//Define N for number of facilities sthat Solution will hold
	~Solution();				//Delete Pointer
	void findCoverage();		//Find Coverage objecive of the Solution
	void findCost();			//find Cost obective of the Solution
};

Solution::Solution() {			//Create Pointer
	Facility* facilitySet = nullptr;

}

Solution::Solution(int N,Facility arr[]) {			//Create Facility Set that holds coordinates of facilities. 
									//Dimension of the facilitySet should be defined externally.
	facilitySet = new Facility[N];
	numFacility = N;

	for (int i = 0; i < N; i++) {

		facilitySet[i].CoordX = arr[i].CoordX;
		facilitySet[i].CoordY = arr[i].CoordY;
	}
}

Solution::~Solution() {				//Delete pointer
	delete[] facilitySet;
}

void Solution::findCoverage() {		//Find Coverage objective

	for (int i = 0; i < numDemand; i++)
	{
		for (int j = 0; j < numFacility; j++)
		{
			if (findDistance(demandSet[i].CoordX,
				demandSet[i].CoordY,
				facilitySet[j].CoordX,
				facilitySet[j].CoordY) < fd)
			{
				Coverage += demandSet[i].Value;
				break;
			}
		}
	}
}

void Solution::findCost() {			//Find Cost Objective
	Cost = numDC * costDC + numRS * costRS;	
}
*/