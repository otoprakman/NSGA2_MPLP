#pragma once
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

FILE
* value,
* coordX,
* coordY,
* writer,
* writeCost,
* writeCoverage,
* writeSumFacility,
* writeCoordX,
* writeCoordY,
* writenumFac,
* writeSumFacilityAfterSelection;

FILE
* selectTime,
* crossoverTime,
* findValueTime,
* keepAliveTime,
* deleteOldTime,
* copyNewGenTime;

std::ifstream inputValue("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonC101\\demand.txt", std::ifstream::in);
std::ifstream inputcoordX("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonC101\\coordx.txt", std::ifstream::in);
std::ifstream inputcoordY("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonC101\\coordy.txt", std::ifstream::in);

void readWrite()
{
	/* open the file for writing*/
	writer = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\FacilityNumberAfterCrossover.txt", "w");
	writeCost = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\HeuristicCost.txt", "w");
	writeCoverage = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\HeuristicCoverage.txt", "w");
	writeSumFacility = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityBeforeSelection.txt", "w");
	writeCoordX = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\Coordx.txt", "w");
	writeCoordY = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\Coordy.txt", "w");
	writenumFac = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\numFac.txt", "w");
	writeSumFacilityAfterSelection = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityAfterSelection.txt", "w");

	selectTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\selectTime.txt", "w");
	crossoverTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\crossoverTime.txt", "w");
	findValueTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\findValueTime.txt", "w");
	keepAliveTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\keepAliveTime.txt", "w");
	deleteOldTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\deleteOldTime.txt", "w");
	copyNewGenTime = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\copyNewGenTime.txt", "w");

	/*Opening the files*/


	for (int i = 0; i < numDemand; i++)
	{
		inputValue >> demandSet.Value[i];
		inputcoordX >> demandSet.CoordX[i];
		inputcoordY >> demandSet.CoordY[i];
	}

	inputValue.close();
	inputcoordX.close();
	inputcoordY.close();
}