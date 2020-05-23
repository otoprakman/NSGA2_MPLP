#pragma once
void M1_mutation(individual* ind_ptr);

void M2_mutation(individual* ind_ptr);

int M2_step1(individual ind, int index, int pred_index);



void M1_mutation(individual* ind_ptr)
{
	//cout << "Individual: " << " Before NumFac: " << ind_ptr->facilitySet.size() << endl;
	auto min_it = std::min_element(ind_ptr->facilitySet.begin(), ind_ptr->facilitySet.end(), find_min);
	ind_ptr->facilitySet.erase(min_it);
	//cout << "Individual: " << " After NumFac: " << ind_ptr->facilitySet.size() << endl;
	//cout << "Individual: " << " Eff: " << efficiency[i]<<" Rnd: "<<rnd << endl;

}

int M2_step1(individual ind, int index, int pred_index) {

	move_points(ind.facilitySet[index], ind.facilitySet[pred_index], findDistance(ind.facilitySet[index].CoordX, ind.facilitySet[index].CoordY,
		demandSet.CoordX[pred_index], demandSet.CoordY[pred_index]));

	int fcov_new = 0;
	if (ind.facilitySet[index].CoordX >= 0.0 && ind.facilitySet[index].CoordY >= 0.0)
	{
		for (int i = 0; i < numDemand; i++)
		{
			if (findDistance(ind.facilitySet[index].CoordX, ind.facilitySet[index].CoordY,
				demandSet.CoordX[i], demandSet.CoordY[i]) < rd) {

				fcov_new += 1;
			}
		}
	}

	return fcov_new - ind.facilitySet[index].nfacCov ;
}

void M2_mutation(individual* ind_ptr)
{
	std::vector<int> is_leaf(ind_ptr->facilitySet.size());
	int pred_node = 0;
	int max = 0;
	int temp = 0;

	int glob_index = 0;
	int glob_pred_node = 0;
	
	bool step1 = 0;

	for (int i = 0; i < ind_ptr->facilitySet.size(); i++)	//Initialization
	{
		is_leaf[i] = 0;
	}

	for (int i = 0; i < ind_ptr->facilitySet.size(); i++)
	{
		for (int j = i+1; j < ind_ptr->facilitySet.size(); j++)
		{

			if (findDistance(ind_ptr->facilitySet[i].CoordX, ind_ptr->facilitySet[i].CoordY,
				ind_ptr->facilitySet[j].CoordX, ind_ptr->facilitySet[j].CoordY) < rc)
			{
				is_leaf[i] += 1;
				is_leaf[j] += 1;
				pred_node = i;

				if (is_leaf[i] >= 2)		
				{
					break;
				}
			}
		}

		if (is_leaf[i] == 1)
		{
			temp = 0;
			temp = M2_step1(*ind_ptr, i, pred_node);
			if (max < temp)
			{
				max = temp;
				glob_index = i;
				glob_pred_node = pred_node;
				step1 = 1;
			}
		}
	}

	//cout << "Mutation-2::Determine Step-1 or Step2: COMPLETED" << std::endl;
	
	if (step1)
	{
		move_points(ind_ptr->facilitySet[glob_index], ind_ptr->facilitySet[glob_pred_node], 
			findDistance(ind_ptr->facilitySet[glob_index].CoordX, ind_ptr->facilitySet[glob_index].CoordY,
			demandSet.CoordX[glob_pred_node], demandSet.CoordY[glob_pred_node]));
		
		//cout << "Mutation-2::Moving Facilities for Step-1 : COMPLETED" << std::endl;
		
		m2CountS1 += 1;
	}
	else
	{
		//M2_step2();
		//cout << "Mutation-2::Moving Facilities for Step-2 : COMPLETED" << std::endl;
		m2CountS2 += 1;
	}

}

void M3_mutation(individual* ind_ptr) {
	/*for (int i = 0; i < ind_ptr->facilitySet.size(); i++)
	{
		cout << "Before- " << i + 1 << ". Facility CoordX: " << ind_ptr->facilitySet[i].CoordX << " CoordY: " << 
			ind_ptr->facilitySet[i].CoordY << " FacCov: " << ind_ptr->facilitySet[i].facCov << std::endl;
	}*/

	if (ind_ptr->facilitySet.size() >= 3)
	{
		convexHull(ind_ptr->facilitySet, ind_ptr->facilitySet.size());

	}
	

	//for (int i = 0; i < ind_ptr->facilitySet.size(); i++)
	//{
	//	cout << "After- " << i + 1 << ". Facility CoordX: " << ind_ptr->facilitySet[i].CoordX << " CoordY: " <<
	//		ind_ptr->facilitySet[i].CoordY << " FacCov: " << ind_ptr->facilitySet[i].facCov << std::endl;
	//}
	//int stop;
	//std::cin >> stop;
}