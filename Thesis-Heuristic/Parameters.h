#pragma once
#define smallest 0.00000001


//////******* PARAMETER SETTINGS ********//////

const int costDC = 4;			//Cost of Opening a new DC
const int costRS = 1;			//Cost of opening a new RS
const int numDemand = 600;		//Number of Discrete Demand Points
const float fd = 1;				//Distance for Coverage Constraint
const float fp = 2;				//Distance for Connectivity Constraint
const int popSize = 50;			//Number of solutions in the solution space (population of the genetic algorithm)
const float mu = 0.5;
const float sigma = 0.5;


const float minLoc = 0.0;		//For initial population facilities are randomly placed, 
const float maxLoc = 10;		//hence x-y coordinate limits are defined

const int generationNum = 100;		//Total generation

const int  maxfun = 2;				//Number of objectives in the problem (Cost:0 / Coverage:1)
const int maxpop = popSize;			//Array size that stores the individual number at a rank
const int max_numFac = 1000;		//max number of facilities in a solution
const int maxInitFacility = 10;		/*While generating initial population, 
									number of facilities in each solution is randomly determined.
									This parameter is for the maximum number of facilities in each population.*/
const int minInitFacility = 1;		//Minimum number of facilities in initial population


const int maxlimit_facility = 30;   //Allow maximum number of facilities in the mating pool
const int minlimit_facility = 1;	//If it is 1, that indicates individuals that has at least 2 or greater number of facilities can be in the matingpool. 

typedef struct 						//Demand Points
{
	float CoordX[numDemand];
	float CoordY[numDemand];
	int Value[numDemand];
}demand;

typedef struct 						//Facility points
{
	float CoordX;
	float CoordY;
}facility;

typedef struct
{
	float cub_len;					//crowding distances
	int fitness[maxfun];			//fitness[0]:Cost - fitness[1]:Coverage
	int numDC;
	int numRS;
	int numFac;
	facility facilitySet[max_numFac],
		* facility_ptr;
	int rank,						/*Rank of the individual*/
		flag;					    /*Flag for ranking*/

}individual;

typedef struct
{
	int rankindices[popSize][popSize];
	int maxrank;				  /*Maximum rank present in the population*/
	int rankno[maxpop];			  /*record of no. of individuals at a particular rank*/
	individual ind[maxpop],
		* ind_ptr;
}population;

typedef struct
{
	int maxrank;				 /*Maximum rank present in the population*/
	int rankno[maxpop];		     /*record of no. of individuals at a particular rank*/
	individual ind[maxpop*2],
		* ind_ptr;
}matepopulation;


population oldpop,
newpop,
lastpop,
* old_pop_ptr,
* new_pop_ptr,
* last_pop_ptr;

matepopulation
matepop,
* mate_pop_ptr;
/*Defining the population Structures*/

demand demandSet,
* demand_ptr;

