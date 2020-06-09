#pragma once


FILE
* writeEfficiency,
* writeResult,
* writeMutationCount;

std::ifstream inputValue("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\demand.txt", std::ifstream::in);
std::ifstream inputcoordX("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\coordx.txt", std::ifstream::in);
std::ifstream inputcoordY("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\coordy.txt", std::ifstream::in);

void readWrite()
{
	/* open the file for writing*/
	writeEfficiency = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\EfficiencyScores.txt", "w");
	writeResult = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\Result.txt", "w");
	writeMutationCount = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\MutationCount.txt", "w");
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