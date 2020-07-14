#pragma once
#include <chrono>
#include <ctime>    
#include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <direct.h>

FILE
* writeEfficiency,
* writeResult,
* writeMutationCount;

std::string filepath = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results";
std::string filename;
std::string dirname;


std::ifstream inputValue("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\demand.txt", std::ifstream::in);
std::ifstream inputcoordX("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\coordx.txt", std::ifstream::in);
std::ifstream inputcoordY("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\OrhanC600\\coordy.txt", std::ifstream::in);

void createTargetFolder() {
	
	std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	
	std::string currentDate = std::ctime(&currentTime);

	remove_if(currentDate.begin(), currentDate.end(), isspace);

	//std::cout << currentDate <<"-----" << currentDate.substr(0, 10) << currentDate.substr(currentDate.find(":")+1,2)<<std::endl;

	currentDate = currentDate.substr(0, 9)+ currentDate.substr(currentDate.find(":") + 1, 2);

	filename = currentDate + "_" + problemType + "_" + facSecType + "_pop" + std::to_string(popSize) + "_init" + std::to_string(maxInitFacility) +
		"_g" + std::to_string(ngen) + "_m" + mutType + "_tcov" + std::to_string(static_cast<int>(tcov)) + "_tncov" + std::to_string(static_cast<int>(tncov))
		+ "_imp" + std::to_string(improvement);

	std::string pathname = "cd C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results && mkdir ";
	std::string command = pathname + filename;
	std::cout << command << std::endl;
	dirname = "C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\Visualization_Algorithms\\Heuristic_Results\\" + filename;

	system(command.c_str());
}

void readWrite()
{
	createTargetFolder();

	/* open the file for writing*/
	writeEfficiency = fopen((dirname + "\\" + "EfficiencyScores.txt").c_str(), "w");
	writeResult = fopen((dirname + "\\" + "Result.txt").c_str(), "w");
	writeMutationCount = fopen((dirname + "\\" + "MutationCount.txt").c_str(), "w");
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