#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t organizer_ready;
sem_t start_organizer;
sem_t players_allocated;
sem_t players_allowed;
sem_t ref_allocated;
sem_t ref_allowed;
sem_t gameready;
sem_t playgame;
sem_t mutex;
pthread_mutex_t mut[9];


int player_count=0;
int referee_count=0;

void enterAcademy(int type, int num)
{ 
	if(type==0) printf("Player %d enters academy.\n",num);
	else printf("Referee %d enters academy.\n",num);
}

void meetOrganizer(int type, int num)
{
	if(type==0) printf("Player %d meets organizer.\n",num);
	else printf("Referee %d meets organizer.\n",num);
}

void enterCourt(int type, int num)
{
	if(type==0) printf("Player %d enters court.\n",num);
	else printf("Referee %d enters court.\n",num);
}

void warmup(int num)
{
	printf("Player %d starts warmup.\n",num);
}

void adjustEquipment(int num)
{
	printf("Referee %d adjusts equipment.\n",num);
}

void startGame(int num)
{
	printf("Referee %d started game.\n",num);
}

void *Player(void *play)
{
	//ensure that dup values do not occur in print
	pthread_mutex_lock(&mut[0]);

    int playernum=*(int*)play;
	enterAcademy(0,playernum);
	pthread_mutex_unlock(&mut[0]);

	//to ensure that players enter only after the current pair is done
	sem_wait(&players_allowed);

	//safety of function
	pthread_mutex_lock(&mut[1]);
	sem_post(&players_allocated);
	meetOrganizer(0,playernum);
	pthread_mutex_unlock(&mut[1]);

	//makes sure that the organizer has all the three before entering the court
	sem_wait(&organizer_ready);

	//safety of functions
	// sem_wait(&mutex);
	pthread_mutex_lock(&mut[2]);
	enterCourt(0,playernum);
	warmup(playernum);
	// sleep(1);
	sleep((double)0.5);
	// sem_post(&mutex);
	pthread_mutex_unlock(&mut[2]);

	sem_post(&gameready);
}

void *Organizer(void* org)
{
	int n,i=0;

    n=*(int*)org;
    sem_post(&start_organizer);
	for(i=0;i<n;i++)
	{
		//makes sure that he gets both the players before proceeding
		sem_wait(&players_allocated);
		sem_wait(&players_allocated);

		//makes sure he gets the referee before proceeding
		sem_wait(&ref_allowed);

		//tells the player and referee threads that all of the three required are present
		sem_post(&organizer_ready);
		sem_post(&organizer_ready);
		sem_post(&organizer_ready);

		//play game
		sem_wait(&playgame);
		//sets the players and referees so that they are ready to accept once again.
		sem_post(&players_allowed);
		sem_post(&players_allowed);
		sem_post(&ref_allocated);
	}
}

void *Referee(void* ref)
{
	//mutex to ensure that the statement is printed immediately after assigning so that multiple same values do not occur
	pthread_mutex_lock(&mut[3]);
    int refnum=*(int*)ref;
	enterAcademy(1,refnum);
	pthread_mutex_unlock(&mut[3]);

	//so that while 1 referee is functioning another does not enter until the organizer is free.When organizer becomes free signal to ref_allocated is also sent
	sem_wait(&ref_allocated);

	//two threads do not access print at the same time
	pthread_mutex_lock(&mut[4]);
	sem_post(&ref_allowed);
	meetOrganizer(1,refnum);
	pthread_mutex_unlock(&mut[4]);

	//this waits until the organizer accumulates 2 players and a referee because that is when the organizer signal is first sent
	sem_wait(&organizer_ready);

	//to ensure the safety of both the prints
	pthread_mutex_lock(&mut[5]);
	enterCourt(1,refnum);
	adjustEquipment(refnum);
	sleep(1);
	pthread_mutex_unlock(&mut[5]);

	sem_wait(&gameready);
	sem_wait(&gameready);

	//safety of function
	pthread_mutex_lock(&mut[6]);
	startGame(refnum);
	pthread_mutex_unlock(&mut[6]);

	sem_post(&playgame);
}


int main()
{
	srand(time(NULL));
	int n,randnum,i,playernum,refereenum;
	for(i=0;i<9;i++) 
	{
		pthread_mutex_init(&mut[i],NULL);
	}
	float pro;
	int prob;
	scanf("%d",&n);
	playernum=2*n;
	refereenum=n;
    int playernumber[100],refnumber[100],plnum,rnum;
	pthread_t organizer_id,player_id[100],ref_id[100];
	sem_init(&players_allowed,0,2);
	sem_init(&players_allocated,0,0);
	sem_init(&organizer_ready,0,0);
    sem_init(&start_organizer,0,0);
	sem_init(&mutex,0,1);
	sem_init(&ref_allocated,0,1);
	sem_init(&ref_allowed,0,0);
	sem_init(&gameready,0,0);
	sem_init(&playgame,0,0);
	pthread_mutex_lock(&mut[8]);
	pthread_create(&organizer_id,NULL,Organizer,(void*)&n);
	pthread_mutex_unlock(&mut[8]);
	int playersent=0;
	int refereesent=0;
    sem_wait(&start_organizer);
	for(i=0;i<3*n;i++)
	{
		sleep(rand()%3);
		// sleep(1);
		pthread_mutex_lock(&mut[7]);
		randnum=rand()%100;
		pro=(playernum)/(float)(playernum+refereenum);
		prob=pro*100;

		if(prob>randnum)
		{
            playersent++;
            plnum=playersent;
            playernumber[plnum]=plnum;
			pthread_create(&player_id[playersent],NULL,Player,(void*)&playernumber[plnum]);
			playernum--;
			
		}
		else
		{
            refereesent++;
            rnum=refereesent;
            refnumber[rnum]=rnum;
			pthread_create(&ref_id[refereesent],NULL,Referee,(void*)&refnumber[rnum]);
			refereenum--;
			
		}
		pthread_mutex_unlock(&mut[7]);
	}
	pthread_join(organizer_id,NULL);
	for(i=0;i<n;i++) pthread_join(ref_id[i+1],NULL);
	for(i=0;i<2*n;i++) pthread_join(player_id[i+1],NULL);
	return 0;
}
