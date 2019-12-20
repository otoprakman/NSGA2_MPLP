#pragma once



//////******* PARAMETER SETTINGS ********//////

const int costDC = 4;			//Cost of Opening a new DC
const int costRS = 1;			//Cost of opening a new RS
const int numDemand = 324;		//Number of Discrete Demand Points
const float fd = 5;				//Distance for Coverage Constraint
const float fp = 2;				//Distance for Connectivity Constraint
const int popSize = 50;	//Number of solutions in the solution space (popuation of the genetic algorithm)
const float mu = 0.5;
const float sigma = 0.1;


const float minLoc = 0.0;		//For initial population facilities are randomly placed, 
const float maxLoc = 10;		//hence x-y coordinate limits are defined

const int gener = 100;

const int  maxfun = 2;			//Number of objectives in the problem
const int maxpop = 50;			//Array size that stores the individual number at a rank
const int max_numFac = 100;		//max number of facilities in a solution
const int maxInitFacility = 10;	//While generating initial population, 
								//number of facilities in each solution is randomly determined.
								//This parameter maximum number of facilities in each population. Min=0


typedef struct 					//Demand Points
{
	float CoordX[numDemand];
	float CoordY[numDemand];
	int Value[numDemand];
}demand;

typedef struct 					//Facility points
{
	float CoordX;
	float CoordY;
}facility;

typedef struct
{
	float cub_len;		 //crowding distances
	int fitness[maxfun]; //fitness[0]:Cost - fitness[1]:Coverage
	int numDC;
	int numRS;
	int numFac;
	facility facilitySet[max_numFac],
		* facility_ptr;
	int rank,              /*Rank of the individual*/
		flag;              /*Flag for ranking*/

}individual;

typedef struct
{
	int maxrank;            /*Maximum rank present in the population*/
	float rankrat[maxpop];  /*Rank Ratio*/
	int rankno[maxpop];     /*Individual at different ranks*/
	individual ind[maxpop],
		* ind_ptr;
}population;


population oldpop,
newpop,
matepop,
* old_pop_ptr,
* new_pop_ptr,
* mate_pop_ptr;
/*Defining the population Structures*/

demand demandSet,
* demand_ptr;



/*INPUT VARIABLES

string facX = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Thesis-Codes\\Thesis-Codes\\ClusteredGrid\\Coordinates_X(Facility).txt";
string facY = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Thesis-Codes\\Thesis-Codes\\ClusteredGrid\\Coordinates_Y(Facility)";
string custX = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Thesis-Codes\\Thesis-Codes\\ClusteredGrid\\Coordinates_X(Customer)";
string custY = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Thesis-Codes\\Thesis-Codes\\ClusteredGrid\\Coordinates_Y(Customer)";

////////////////////////
*/
