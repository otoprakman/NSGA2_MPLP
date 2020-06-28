#pragma once
#include <string>
#define smallest 0.00000001

//ALGORITHM PARAMETER SETS//
std::string problemType = "C600";
std::string facSecType = "Dynamic";
std::string mutType = "2";

//////******* PARAMETER SETTINGS ********//////

std::string RPath;					// Path of R.exe

const bool Plot = 1;
const bool adaptive = 0;			//Adaptive Facility Selection
const bool dynamic = 1;				//Dynamic Facilty Selection
const bool normal = 0;				//Normal Facility Selection
const bool improvement = 1;			//Use improvement
const int numSnap = 100;

const int costDC = 4;				//Cost of opening a new DC
const int costRS = 1;				//Cost of opening a new RS
const float rd = 1.0;				//Distance for Coverage Constraint
const float rc = 2.0;				//Distance for Connectivity Constraint
const int popSize = 50;				//Number of solutions in the solution space (population of the genetic algorithm)
const float mu = 0.5;				//Generating random numbers from normal distribution, mu is the parameter of the normal dist.
const float sigma = sqrt(0.5);		//parameter of the normal distribution
const int ngen = 1000;				//Total generation
const int maxInitFacility = 50;		/*While generating initial population, 
									number of facilities in each solution is randomly determined.
									This parameter is for the maximum number of facilities in each population.*/
const int minInitFacility = 1;		//Minimum number of facilities in initial population

//////******* PROBLEM SETTINGS ********//////
const int numDemand = 600;			//Number of Discrete Demand Points
const float minLoc = 0.0;			//For initial population facilities are randomly placed, 
const float maxLoc = 10.0;			//hence x-y coordinate limits are defined

//////******* VARIABLES ********//////

const int maxfun = 2;				//Number of objectives in the problem (Cost:0 / Coverage:1)

float encov[popSize];				//Nonoverlapping coverage efficiency of an individual
float avg_fac;						//Mean Facility at the population
float ecov[popSize];				//Demand coverage efficiency of an individual

float tcov = 0.6;					//Threshold for overlapping coverage
float tncov = 0.8;					//Threshold for non-overlapping coverage

//////******* COUNT VARIABLES ********//////

int m1Count = 0;
int m2CountS1 = 0;
int m2CountS2 = 0;
int m3Count = 0;

int g_counter = 0;

//////******* TIME VARIABLES ********//////
float nondominatedsortTime = 0;
float findDistTimer = 0;
clock_t DistTimer;
clock_t ender;

float initTime = 0;
float crossTime = 0;
float covTime = 0;

float costTime = 0;
float totaltime = 0;
float fselectiontime = 0;
float mutationtime = 0;
clock_t starting;
clock_t ending;
clock_t startingAll;
clock_t endingAll;

//////******* FUNDAMENTAL VARIABLES ********//////

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
	int facCov;
	int nfacCov;					//nfacCov: individual coverage of the facility. not considering overlapping coverage
}facility;

typedef struct
{
	float cub_len;					//crowding distances
	int fitness[maxfun];			//fitness[0]:Cost - fitness[1]:Coverage
	int numDC;						//number of Distribution Centers
	int numRS;						//number of RRecharge Stations
	int numFac;						//total number of facilities
	std::vector<facility> facilitySet;//set of facilities in the individual
	facility* facility_ptr;
	int rank,						/*Rank of the individual*/
		flag;					    /*Flag for ranking*/
	float avg_dis;				//Average distance between facilities totaldist/(totalfacility-1)

}individual;

typedef struct
{
	int rankindices[popSize][popSize];
	int maxrank;					  /*Maximum rank present in the population*/
	int rankno[popSize];			  /*record of no. of individuals at a particular rank*/
	individual ind[popSize],
		* ind_ptr;

}population;

typedef struct
{
	int maxrank;					 /*Maximum rank that is present in the population*/
	int rankno[popSize];		     /*record of no. of individuals at a particular rank*/
	individual ind[popSize*2],
		* ind_ptr;

}matepopulation;

//////******* GLOBAL VARIABLES ********//////

population oldpop,
newpop,
lastpop,
* old_pop_ptr,
* new_pop_ptr,
* last_pop_ptr;

matepopulation
matepop,
* mate_pop_ptr;

demand demandSet;


