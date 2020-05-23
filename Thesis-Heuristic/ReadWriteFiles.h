#pragma once


FILE
* writeBeforeSelection,
* writeAfterSelection,
* writeEfficiency,
* writeResult,
* writeMutationCount,
* writeOffspringEfficiency,
* writeafterMutOffEff;

std::ifstream inputValue("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonRC101\\demand.txt", std::ifstream::in);
std::ifstream inputcoordX("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonRC101\\coordx.txt", std::ifstream::in);
std::ifstream inputcoordY("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\SolomonRC101\\coordy.txt", std::ifstream::in);

void readWrite()
{
	/* open the file for writing*/
	writeBeforeSelection = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityBeforeSelection.txt", "w");
	writeAfterSelection = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\SumFacilityAfterSelection.txt", "w");
	writeEfficiency = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\EfficiencyScores.txt", "w");
	writeResult = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\Result.txt", "w");
	writeMutationCount = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\MutationCount.txt", "w");
	writeOffspringEfficiency = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\OffspringEfficiencyScores.txt", "w");
	writeafterMutOffEff = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\writeafterMutOffEff.txt", "w");
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