/*This is the file to get the different individuals selected*/

std::default_random_engine generator;
std::uniform_real_distribution<> distribution(0,1);

void nselect(population* old_pop_ptr, matepopulation* pop2_ptr);

individual* selectParent(population* pop_ptr);


/********************************************************/


individual* selectParent(population* pop_ptr)       //Select parent randomly;
{
    float rnd = distribution(generator)*popSize;
    int index = round(rnd);
 
    if (index == popSize) pop_ptr->ind_ptr = &(pop_ptr->ind[index - 1]);
         else pop_ptr->ind_ptr = &(pop_ptr->ind[index]);

    return pop_ptr->ind_ptr;
}


void nselect(population* old_pop_ptr, matepopulation* pop2_ptr)
{
    int* fit_ptr1, * fit_ptr2;
    individual* parent1, *parent2;

  float *f1_ptr,*f2_ptr;
  
  old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);
  
  pop2_ptr->ind_ptr= &(pop2_ptr->ind[0]); 

  for(int n = 0; n < 2*popSize; n++)
    {
      /*Select first parent randomly*/

      parent1 = selectParent(old_pop_ptr);
      //cout << "Parent-1: " << parent1->numFac << endl;

      
      /*Select second parent randomly*/

      parent2 = selectParent(old_pop_ptr);
      //cout << "Parent-2: " << parent2->numFac << endl;

      
      old_pop_ptr->ind_ptr = parent1;
      
      fit_ptr1 = &(old_pop_ptr->ind_ptr->rank);
     // printf("Rank1:%d--%d\n", parent1->numDC, *fit_ptr1);
      f1_ptr = &(old_pop_ptr->ind_ptr->cub_len);
    
      old_pop_ptr->ind_ptr = parent2;

      fit_ptr2 = &(old_pop_ptr->ind_ptr->rank);
      //printf("Rank2:%d--%d\n",parent2->numDC, *fit_ptr2);
      f2_ptr = &(old_pop_ptr->ind_ptr->cub_len);
/*--------------------------------------------------------------------------*/
 //         HANGI INDIVIDUAL'LARI SEÇTİĞİNİ GÖRMEK İÇİN YAZDIRIYORUM
 /*         printf("Cost:%d-Coverage:%d-NumFac:%d-Rank:%d-Crowding:%f\nCost:%d-Coverage:%d-NumFac:%d-Rank:%d-Crowding:%f\n",
              parent1->fitness[0], parent1->fitness[1], parent1->numFac, parent1->rank, parent1->cub_len,
              parent2->fitness[0], parent2->fitness[1], parent2->numFac, parent2->rank, parent2->cub_len);
 */
/*------------------SELECTION PROCEDURE------------------------------------*/
      
      /*Comparing the fitnesses*/
      
      if(*fit_ptr1 > *fit_ptr2) pop2_ptr->ind[n] = *parent2;
         else{
	        if(*fit_ptr1 < *fit_ptr2) pop2_ptr->ind[n] = *parent1; 
	        else
	        {
	            if(*f1_ptr <= *f2_ptr) pop2_ptr->ind[n] = *parent2;  /// *orjinalde parent1 olması gerekiyor ancak extreme soln'lar en yüksek cub_len atanıyor algoritmada
	            else pop2_ptr->ind[n] = *parent1;
	        }
	     } //end if
      //cout << "Selected Parent: "<< pop2_ptr->ind[n].numFac << endl;
 /*     printf("(Selected)Cost:%d-Coverage:%d-NumFac:%d,Crowding:%f\n",
          pop2_ptr->ind[n].fitness[0], pop2_ptr->ind[n].fitness[1], pop2_ptr->ind[n].numFac, pop2_ptr->ind[n].cub_len);*/
    }//end for

  old_pop_ptr->ind_ptr = &(old_pop_ptr->ind[0]);

  pop2_ptr->ind_ptr = &(pop2_ptr->ind[0]);

}







