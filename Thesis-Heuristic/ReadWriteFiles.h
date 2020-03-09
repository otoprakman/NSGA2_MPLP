#pragma once
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

FILE
* writeBeforeSelection,
* writeAfterSelection,
* writeEfficiency,
* writeResult;

std::ifstream inputValue("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\demand.txt", std::ifstream::in);
std::ifstream inputcoordX("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\coordx(Customer).txt", std::ifstream::in);
std::ifstream inputcoordY("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Cluster\\coordy(Customer).txt", std::ifstream::in);

void readWrite()
{
	/* open the file for writing*/
	writeBeforeSelection = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityBeforeSelection.txt", "w");
	writeAfterSelection = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityAfterSelection.txt", "w");
	writeEfficiency = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\EfficiencyScores.txt", "w");
	writeResult = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\Result.txt", "w");

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