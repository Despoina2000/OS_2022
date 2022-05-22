#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Mutexes
pthread_mutex_t mutex_telephonist= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cashier = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seats = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_printer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_balance= PTHREAD_MUTEX_INITIALIZER;

//Condition
pthread_cond_t cond_telephonist = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cashier = PTHREAD_COND_INITIALIZER;

//Variable
int *zoneA[ Nseat,NzoneA];
int *zoneB[ Nseat, NzoneB];
int telephonist;
int cashier;
unsigned int seed;
double balance;
double sum_waiting_time;
double sum_service_time;

void * processing(void * customer_id){

    struct timespec timer_on;
	struct timespec	service_timer;
	struct timespec timer_off;
    int id=*(int *)customer_id;

    clock_gettime(CLOCK_REALTIME, &timer_on); //Start timer
    pthread_mutex_lock(&mutex_telephonist);
    while(telephonist <= 0)
	{
	pthread_cond_wait(&cond_telephonist ,&mutex_telephonist);
	}
	--telephonist;
	clock_gettime(CLOCK_REALTIME, &service_timer);
    pthread_mutex_unlock(&mutex_telephonist);

    int choosen_seats = rand_r(&seed) % Nseathigh + Nseatlow;
	int respond_time = rand_r(&seed) % tseathigh + tseatlow;
    bool possible_zone= rand_r(&seed) % 100 / 100.0f <= PzoneB;
	sleep(respond_time);

    pthread_mutex_lock(&mutex_seats);
    bool flag_success=false;
    if(possible_zone){//searching zone A
        int sum[NzoneA];
         
	for(int i = 0; i<Nseat;i++)
	{   sum[i]=0
        for(int j=0; j<NzoneA; j++){
        if(zoneA[i][j] == 0) sum[i]++;
    }
		if(sum[i]>= choosen_seats){
            flag_success=true;
            count = 0;
		for(int j = 0; j<NzoneA;j++)
		{
			if(zoneA[i][j] == 0 AND count<sum[i])
			{
		    		zoneA[i] = id;
		    		count++;
	 		}
			 else if (count>sum[i]) break;
        }
		break;

	}
    }
    


}else{//searching zone B
     int sum[NzoneB];
         
	for(int i = 0; i<Nseat;i++)
	{   sum[i]=0
        for(int j=0; j<NzoneB; j++){
        if(zoneA[i][j] == 0) sum[i]++;
    }
		if(sum[i]>= choosen_seats){
            flag_success=true;
            count = 0;
		for(int j = 0; j<NzoneB;j++)
		{
			if(zoneB[i][j] == 0 AND count<sum[i])
			{
		    		zoneB[i] = id;
		    		count++;
	 		}
			 else if (count>sum[i]) break;
        }
		break;
	}
    }
    }
    
    if(flag_success){
	pthread_mutex_lock(&mutex_cashier);
	while(cashier <= 0)
	{
	pthread_cond_wait(&cond_cashier ,&mutex_cashier);
	}
	cashier--;
        if(rand_r(&seed) % 100 / 100.0f <= Pcardsucces){
            pthread_mutex_lock(&mutex_balance);
            if(possible_zone)
            balance += choosen_seats * CzoneA;
            else balance += choosen_seats * CzoneB;
	    pthread_mutex_unlock(&mutex_balance);
        }
    }
    else{
        pthread_mutex_lock(&mutex_seats);
        //unlock seats
	    if(possible_zone){
	    for(int i = 0; i<Nseat;i++)
		{   flag_right_line =false;
        	for(int j=0; j<NzoneA; j++){
        	if(zoneA[i][j] == id) {zoneA[i][j];flag_right_line =true;}
		}
		 if (flag_right_line) break;
	      }
	    }
	    else{
	    for(int i = 0; i<Nseat;i++)
		{   flag_right_line =false;
        	for(int j=0; j<NzoneB; j++){
        	if(zoneB[i][j] == id) {zoneB[i][j];flag_right_line =true;}
		}
		 if (flag_right_line) break;
	      }
	    }
        pthread_mutex_unlock(&mutex_seats);
    }
    
    }

int main(int argc, char *argv[]){
    
}
