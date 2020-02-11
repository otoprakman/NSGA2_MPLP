#pragma once
#include <sstream>
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

void readWrite()
{
	/* open the file for writing*/
	writer = fopen("FacilityNumberAfterCrossover.txt", "w");
	writeCost = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\HeuristicCost.txt", "w");
	writeCoverage = fopen("C:\\Users\\ThinkPad\\Desktop\\Thesis\\Codes\\Visualization_Algorithms\\HeuristicCoverage.txt", "w");
	writeSumFacility = fopen("SumFacility.txt", "w");
	writeCoordX = fopen("CoordX.txt", "w");
	writeCoordY = fopen("CoordY.txt", "w");
	writenumFac = fopen("numFac.txt", "w");
	writeSumFacilityAfterSelection = fopen("writeSumFacilityAfterSelection.txt", "w");

	selectTime = fopen("selectTime.txt", "w");
	crossoverTime = fopen("crossoverTime.txt", "w");
	findValueTime = fopen("findValueTime.txt", "w");
	keepAliveTime = fopen("keepAliveTime.txt", "w");
	deleteOldTime = fopen("deleteOldTime.txt", "w");
	copyNewGenTime = fopen("copyNewGenTime.txt", "w");

	fopen_s(&value, "demand_value.txt", "r");
	fopen_s(&coordX, "demand_coordX.txt", "r");
	fopen_s(&coordY, "demand_coordY.txt", "r");
	/*Opening the files*/


	for (int i = 0; i < numDemand; i++)
	{

		demand_ptr = &(demandSet);
		fscanf(value, "%d\n", &(demand_ptr->Value[i]));
		fscanf(coordX, "%f\n", &(demand_ptr->CoordX[i]));
		fscanf(coordY, "%f\n", &(demand_ptr->CoordY[i]));

	}

	fclose(value);
	fclose(coordX);
	fclose(coordY);
}