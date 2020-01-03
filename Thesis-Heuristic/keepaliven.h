/*This is a routine to keep the fronts alive (caring the end problem)*/
#include "Parameters.h";
#include <stdio.h>

typedef struct
{
  int maxrank,   /*Max rank of the global population*/
    rankindices[2*popSize][2*popSize], /*record of array of individual numbers at
				  a particular rank [rank][individual index] */
    rankno[2*maxpop];           /*record of no. of individuals at a particular rank*/

  individual ind[2*maxpop],
      * ind_ptr;
}globpop;

/*Population structure for the pool having both the old as well as new 
  population*/

globpop globalpop,*global_pop_ptr;

void grank(globpop* global_pop_ptr);
/*Ranking the global pool*/

//
//int indcmp1(int *ptr1,int *ptr2);
///*Comparison of the variables*/

void gsort(int rnk,int sel);
/*Sorting for the function values in ascending order*/

void gshare(int rnk);
/*Sharing the fitness*/

void finalsort(int rnk);

int left,Lastrank;
float fparaglob[2*maxpop][2];

void keepalive(population *pop1_ptr,population *pop2_ptr,population *pop3_ptr,int gen)
{
  int i,j,k,m,a1,l,rec;
  
  int st,pool,poolf,sel;
  
  /*Forming the global mating pool*/
  for (i = 0; i < popSize; i++)
  {
           /*Fitness is copied to the global pool */
      for (l = 0; l < maxfun; l++)
      {
          globalpop.ind[i].fitness[l] = pop1_ptr->ind[i].fitness[l];
          globalpop.ind[i + popSize].fitness[l] = pop2_ptr->ind[i].fitness[l];
      }
      globalpop.ind[i].numDC = pop1_ptr->ind[i].numDC;
      globalpop.ind[i+popSize].numDC = pop2_ptr->ind[i].numDC;

      globalpop.ind[i].numRS = pop1_ptr->ind[i].numRS;
      globalpop.ind[i + popSize].numRS = pop2_ptr->ind[i].numRS;

      globalpop.ind[i].numFac = pop1_ptr->ind[i].numFac;
      globalpop.ind[i + popSize].numFac = pop2_ptr->ind[i].numFac;
      for (int z = 0; z < max_numFac; z++)
      {
          globalpop.ind[i].facilitySet[z] = pop1_ptr->ind[i].facilitySet[z];
          globalpop.ind[i+popSize].facilitySet[z] = pop2_ptr->ind[i].facilitySet[z];
      }
      /*Initialising the dummyfitness to zero */
      globalpop.ind[i].cub_len = 0;
      globalpop.ind[i + popSize].cub_len = 0;
  }

  global_pop_ptr = &(globalpop);
  
  /*Finding the global ranks */
    grank(global_pop_ptr);

	m = globalpop.maxrank;
  
  /* Sharing the fitness to get the dummy fitness */
  for(i = 0;i < m;i++)
    {
      gshare(i+1);
    }
 
  poolf = popSize;
  pool = 0;
  
  /*Initializing the flags of population to zero */
  for(i = 0; i < 2*popSize;i++)
    {
      globalpop.ind[i].flag = 0;
    }
  // decide which all solutions belong to the pop3 
  rec = 0;
  st = 0;
  for(i = 0 ;i < m ; i++)
  {
      /*    Elitism Applied Here     */
      st = pool;
      pool += globalpop.rankno[i];
      
      if(pool <= popSize)
	{
	  for(k = 0;k < 2*popSize ;k++)
	    {
	      if(globalpop.ind[k].rank == i+1)
		globalpop.ind[k].flag = 1;
	    }
	  pop3_ptr->rankno[i] = globalpop.rankno[i];
	}
      else
	{
	  sel = popSize - st;
	  Lastrank = i+1;
	  pop3_ptr->rankno[i] = sel;
	  gsort(i+1,sel);
	  break;
	}
  }


  k = 0;
  for (i = 0, k = 0; i < 2 * popSize && k < popSize; i++)
  {

	  if (globalpop.ind[i].flag == 1)
      {
          for (j = 0; j < maxfun; j++) 
			  pop3_ptr->ind[k].fitness[j] = globalpop.ind[i].fitness[j];

          pop3_ptr->ind[k].cub_len = globalpop.ind[i].cub_len;
          pop3_ptr->ind[k].rank = globalpop.ind[i].rank;
          pop3_ptr->ind[k].numDC = globalpop.ind[i].numDC;
          pop3_ptr->ind[k].numRS = globalpop.ind[i].numRS;
          pop3_ptr->ind[k].numFac = globalpop.ind[i].numFac;
          for (j = 0; j < pop3_ptr->ind[k].numFac; j++)
              pop3_ptr->ind[k].facilitySet[j] = globalpop.ind[i].facilitySet[j];
          k++;  // increment the pop3 counter
      }
  }

  k = 0;
  pop3_ptr->maxrank = Lastrank;

}

//void grank(int gen)
//{
//  int i,j,k,rnk,val,nondom,popsize1,gflg[2*maxpop],q;
//  int *ptr1,*ptr2;
//  FILE *gr;
//  gr = fopen("g_rank_record.out","a");
//  fprintf(gr,"Genration no. = %d\n",gen);
//  /*----------------------------* RANKING *---------------------------------*/
//  rnk = 0;
//  nondom = 0;
//  popsize1 = 2*popSize;
//  
//  for(i = 0;i < popsize1;i++)
//    {
//      gflg[i] = 2;
//    }
//  
//  for(k = 0;k < popsize1;k++)
//    {
//      q =  0;
//      for(j = 0;j < popsize1;j++)
//	{
//	  if (gflg[j] != 1) break;
//	}
//      if(j == popsize1) break;
//      rnk = rnk +1;
//      for( j = 0 ;j < popsize1; j++)
//	{
//	  if(gflg[j] == 0) gflg[j] = 2;
//	}
//      for(i = 0;i < popsize1 ; i++)
//	{
//	  if(gflg[i] != 1 && gflg[i] != 0) 
//	    {
//	      ptr1 = &(global_pop_ptr->ind[i].fitness[0]);
//	      for(j = 0;j < popsize1 ; j++)
//		{
//		  if( i!= j)
//		    {
//		      if(gflg[j] != 1)
//			{
//			  ptr2 = &(global_pop_ptr->ind[j].fitness[0]);
//			  val = indcmp1(ptr1,ptr2);
//			  if( val == 2)
//			    { 
//			      gflg[i] = 0;/* individual 1 is dominated */
//			      break;
//			    }
//			  if(val == 1)
//			    {
//			      gflg[j] = 0;/* individual 2 is dominated */
//			    }
//			  if(val == 3)
//			    {
//			      nondom++;/* individual 1 & 2 are non dominated */
//			      if(gflg[j] != 0)gflg[j] = 3;
//			    }
//			}
//		    }
//		}
//	      if( j == popsize1)
//		{
//		  global_pop_ptr->ind[i].rank = rnk;
//		  gflg[i] = 1;
//		  global_pop_ptr->rankar[rnk-1][q] =  i;
//		  q++;
//		}
//	    }
//	}
//      global_pop_ptr->rankno[rnk-1] = q;
//    } 
//  global_pop_ptr->maxrank = rnk;
//
//  fprintf(gr,"   RANK     No Of Individuals\n");
//  for(i = 0;i < rnk;i++)
//    fprintf(gr,"\t%d\t%d\n",i+1,globalpop.rankno[i]);
//  
//  fclose(gr);
//  return;
//}

void grank(globpop* global_pop_ptr)
{
	int j,
		rnk,           /*rank*/
		val,           /*value obtained after comparing two individuals*/
		nondom,        /*no of non dominated members*/
		maxrank1;      /*Max rank of the population*/


	int* ptr1,		//Pointers for objective values
		* ptr2;

	/*------------------------------* RANKING *------------------------------*/

	/*Initializing the ranks to zero*/
	rnk = 0;

	nondom = 0;
	maxrank1 = 0;

	/*min_fit is initialize to start distributing the dummy fitness =
	  popsize to the rank one individuals and keeping the record such
	  that the minimum fitness of the better rank individual is always
	  greater than max fitness of the relatively worse rank*/


	  /*Difference in the fitness of minimum dummy fitness of better rank
		and max fitness of the next ranked individuals*/

		/*Initializing all the flags to 2*/

			/* Meaning of flag;
			   0: Dominated individual
			   1: Assigned a rank
			   2: Not ranked
			   3: not comparable
			*/
	for (int i = 0; i < 2*popSize; i++)
	{
		global_pop_ptr->ind[i].flag = 2;
	}

	int q = 0;

	for (int k = 0; k < 2*popSize; k++, q = 0)
	{
		for (j = 0; j < 2*popSize; j++)
		{
			if (global_pop_ptr->ind[j].flag != 1)break;
			/*Break if all the individuals are assigned a rank*/
		}
		if (j == 2*popSize)break;

		rnk = rnk + 1;

		for (int j = 0; j < 2*popSize; j++)
		{
			if (global_pop_ptr->ind[j].flag == 0) global_pop_ptr->ind[j].flag = 2;
			/*Set the flag of dominated individuals to 2*/
		}

		for (int i = 0; i < 2*popSize; i++)
		{
			/*Select an individual which rank to be assigned*/

			global_pop_ptr->ind_ptr = &(global_pop_ptr->ind[i]);

			if (global_pop_ptr->ind_ptr->flag != 1 && global_pop_ptr->ind_ptr->flag != 0) // select if not dominated and not assigned
			{
				ptr1 = &(global_pop_ptr->ind_ptr->fitness[0]); //Get cost objective

				for (j = 0; j < 2*popSize; j++)
				{

					/*Select the other individual which has not got a rank*/
					if (i != j)
					{
						if (global_pop_ptr->ind[j].flag != 1)
						{
							global_pop_ptr->ind_ptr = &(global_pop_ptr->ind[j]);
							ptr2 = &(global_pop_ptr->ind_ptr->fitness[0]);
							/*Compare the two individuals for fitness*/
							val = indcmp(ptr1, ptr2);

							/*VAL = 2 for dominated individual which rank to be given*/
							/*VAL = 1 for dominating individual which rank to be given*/

							/*VAL = 3 for non comparable individuals*/

							if (val == 2)
							{
								global_pop_ptr->ind[i].flag = 0;/* individual 1 is dominated */
								break;
							}

							if (val == 1)
							{
								global_pop_ptr->ind[j].flag = 0;/* individual 2 is dominated */

							}

							if (val == 3)
							{
								nondom++;/* individual 1 & 2 are non dominated */
								if (global_pop_ptr->ind[j].flag != 0)
									global_pop_ptr->ind[j].flag = 3;
							}

						}   /*if loop ends*/
					}       /* i != j loop ends*/
				}           /*loop over j ends*/

				if (j == 2*popSize)
				{
					/*Assign the rank and set the flag*/
					global_pop_ptr->ind[i].rank = rnk;
					global_pop_ptr->ind[i].flag = 1;
					global_pop_ptr->rankindices[rnk - 1][q] = i;
					q++;
				}
			}       /*Loop over flag check ends*/
		}           /*Loop over i ends */

		global_pop_ptr->rankno[rnk - 1] = q;

	}

	maxrank1 = rnk;

	/* Find Max Rank of the population    */
	for (int i = 0; i < popSize; i++)
	{
		rnk = global_pop_ptr->ind[i].rank;

		if (rnk > maxrank1)maxrank1 = rnk;

	}

	global_pop_ptr->maxrank = maxrank1;
}





//int indcmp1(int *ptr1,int *ptr2)
//{
//  int fit1[maxfun],fit2[maxfun];
//  int i,value,m,n;
//  for(i = 0;i < maxfun ;i++)
//    {
//      fit1[i] = *ptr1++;
//      fit2[i] = *ptr2++;
//    }
//
//  m = 0;n=0;
//  while(m < maxfun && fit1[m] <= fit2[m]) 
//    {
//      if((fit2[m] - fit1[m]) < 1e-7) n++;
//      m++;
//    }
//  if(m == maxfun) 
//    {
//      if(n == maxfun) value = 3;
//      else value = 1;                    /*value = 1 for dominating*/
//    }
//  else 
//    {
//      m = 0;n = 0;
//      while(m < maxfun && fit1[m] >= fit2[m]) 
//	{
//	  if((fit1[m] - fit2[m]) < 1e-7) n++;
//	  m++;
//	}
//      if(m == maxfun)
//	{
//	  if(n != maxfun)
//	    value = 2;                       /*value =  2 for dominated */
//	  else value = 3;
//	}
//      else value = 3;                   /*value = 3 for incomparable*/
//    }
//  return value;
//}


/* This is the file used to sort the dummyfitness arrays */
void gsort(int rnk,int sel)
{
  int i,j,a,q;
  float array[2*maxpop][2],temp,temp1;
  
  q = globalpop.rankno[rnk-1];
  
  for(i = 0 ;i < q ;i++)
    {
      array[i][0] = globalpop.rankindices[rnk-1][i];
      a = globalpop.rankindices[rnk-1][i];
      array[i][1] = globalpop.ind[a].cub_len;
    }
  for(i = 0;i < q ;i++)
    {
      for(j = i+1;j < q;j++)
	{
	  if(array[i][1] > array[j][1]) //Orjinal kodda "<" işareti var ancak bu modelde ">" uygun oluyor???
	    {
	      temp = array[i][1];
	      temp1 = array[i][0];
	      array[i][1] = array[j][1];
	      array[i][0] = array[j][0];
	      
	      array[j][1] = temp;
	      array[j][0] = temp1;
	    }
	}
    }
  
  for(i = 0;i < sel;i++)
    {
      a = array[i][0];
      globalpop.ind[a].flag = 1;
    }
  return;
}

/*========================================================================*/

void gshare(int rnk)
{
  float length[2*maxpop][2],max;
  int i,j,m1,a ;
  float min, Diff;  // Added 18.08.2003

  m1 = globalpop.rankno[rnk-1];
  
  for(j = 0;j < maxfun;j++)
    {
      for(i = 0;i < m1;i++)
	{
	  fparaglob[i][0] = 0;
	  fparaglob[i][1] = 0;
	}
      
      for(i = 0;i < m1;i++)
	{
	  a = globalpop.rankindices[rnk-1][i];
	  fparaglob[i][0] = (float)a ;
	  fparaglob[i][1] = globalpop.ind[a].fitness[j];
	}
      
      finalsort(m1); /*Sort the arrays in ascending order of the fitness*/
      
      max = fparaglob[m1-1][1];
      min = fparaglob[0][1];  // Added 18.08.2003
      Diff = max-min;      // Added 18.08.2003 and 5 subsequent lines
      if (Diff < 0.0) 
	  {
	    printf("Something wrong in keepaliven.h\n");
	    exit(1);
      }
     
      for(i = 0;i < m1;i++)
	{
	  if(i == 0 ||i == (m1-1))
	    { 
	      length[i][0] = fparaglob[i][0];
	      length[i][1] = 100*max;
	    }
	  else
	    {
	      length[i][0] = fparaglob[i][0];
	      length[i][1] = fabs(fparaglob[i+1][1]- fparaglob[i-1][1])/(Diff+smallest); // crowding distances are normalized 18.08.2003
	    }
	}
      for(i = 0;i < m1;i++)
	{
	  a = length[i][0];
	  globalpop.ind[a].cub_len += length[i][1];

	}
    }
  
  return;
}


void finalsort(int m1)
{
  float temp,temp1; 
  int i1,j1,k1;
  for(k1 = 0;k1 < m1-1;k1++)
    {
      for(i1 = k1+1;i1 < m1;i1++)
	{
	  if(fparaglob[k1][1] > fparaglob[i1][1])
	    {
	      temp = fparaglob[k1][1];
	      temp1 = fparaglob[k1][0];
	      fparaglob[k1][1] = fparaglob[i1][1];
	      fparaglob[k1][0] = fparaglob[i1][0];
	      fparaglob[i1][1] = temp;
	      fparaglob[i1][0] = temp1;
	    }
	}
    }
  return;
}
