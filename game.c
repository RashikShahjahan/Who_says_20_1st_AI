
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int  score;   /* Globals */
char in[20];

int user(void)
{
 int i=0;
 while(i!=1 && i!=2){
    printf("We are at %-2d, add 1 or 2 ? ",score);
    fgets(in,10,stdin);
    i=atoi(in);
  };
 return(i);
}


int computer(void)
{
 int i;
 if (score% 3==1)  i=1;
 else if (score %3==0)  i=2;
 else  i=rand() %2+1;
 printf("We are at %-2d Computer adds %d \n",score,i);
 return(i);
}






double QVal[20][2]; // STATES AND ACTIONS




void initialize_q_table(void){
	
	for(int i = 0; i<20; i++){
		for(int j = 0; j<2; j++){
		QVal[i][j] =  0.;
		}
	}	

}

int is_end(int score)
{
        if(score>=20){return(1);} 
        return(0);
}




double eps;

int choose_action(int score)
{
	int act;

	double ex=(double)rand() / (double)RAND_MAX ;

	if(ex>eps){
		return(rand()%2+1);
	}

	// CHOOSE  GREEDY to Policy
	int a;
	int b_act=0;
	double max=-1000;
	for(a=1;a<=2;a++){//all actions find max action
		if(QVal[score][a-1]>max){
			b_act=a;
			max=QVal[score][a-1];
		}
	}
	act=b_act;
	return(act);
}

int go_next_state(int* score, int action)
	{
		*score = *score+action;
		if(*score>=20) {printf(" YOU WIN !!\n ");return 100;};
		*score=*score+computer();
    		if(*score>=20) {printf(" I WIN !! \n ");return -100;};
		return 0;
		
	}

int policy[20];

void play(void){
  srand(time(NULL)); 
  int i;
  printf(" Who says first 20 \n \n");
  score=0;
  i=0;
  while(i!=1 && i!=2){
    printf("Who goes first: you=1 computer=2 ? ");
    fgets(in,10,stdin);
    i=atoi(in);
  };
  if(i==2)score=policy[score];
  while(score <=20){
    score=score+user();
    if(score>=20) {printf(" YOU WIN !!\n ");break;};
    score=score+policy[score];
    if(score>=20) {printf(" I WIN !! \n ");break;};
 };
}

void make_policy(void)
{
        double m_rew;
	int m_act;
        int i,a;
        for(i=0;i<20;i++){
                
			m_rew=-100000;
			m_act=0;
                        for(a=0;a<2;a++){//ALL ACTIONS
				if(QVal[i][a]>m_rew){
					m_rew=QVal[i][a];
					m_act=a+1;
				}	
			
                }
		policy[i]=m_act;
		printf("%d\t",i);
		printf("%d\n",policy[i]);
        }

}




#define ALPHA  .2
#define GAMMA  .9
// ONE STEP OF Q-LEARNING
void iterate(void)
{
	int act,a;
	int i;
	
	
	while(1){
		
		act=choose_action(score);
		printf("%d\t",act);
		i = score;
		
		int reward=go_next_state(&score,act);
				
		// FIND MAX
		double max=-1000.;
		for(a=0;a<2;a++){
			if(QVal[score][a]>max)max=QVal[score][a];
		}
		QVal[i][act-1]+= ALPHA*(  reward  + GAMMA*(max - QVal[i][act-1]));
		
		if(is_end(score)){printf("NEXT EP \n");break;};
	}

	// Decay exploration
	eps=eps*0.99;
	if(eps<0.01)eps=0.01;
}

#define NUM_ITER  1000

int main(void)
{
	int i;
	srand(time(0));
	eps = 1;

	for(i=0;i<NUM_ITER;i++){
		score = 0;
		iterate();
		
	}
	
	
	printf("Learnt policy:\n");
	make_policy();
	play();

	return(0);
}