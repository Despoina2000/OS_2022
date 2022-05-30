#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Mutexes
pthread_mutex_t mutex_telephonist= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cashier = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seatA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_seatB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_listA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_listB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_printer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_balance= PTHREAD_MUTEX_INITIALIZER;

//Condition
pthread_cond_t cond_telephonist = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_cashier = PTHREAD_COND_INITIALIZER;

//Variable
int *zoneA[ NzoneA, Nseat];
int *zoneB[ NzoneB, Nseat];
struct Node* listA[ NzoneA];
struct Node* listB[ NzoneB];
int telephonist;
int cashier;
unsigned int seed;
double balance;
double sum_waiting_time;
double sum_service_time;
int sum_typeA;// κράτηση ολοκληρώθηκε επιτυχώς
int sum_typeB;// κράτηση απέτυχε γιατί δεν υπάρχουν κατάλληλες θέσεις
int sum_typeC;// κράτηση απέτυχε γιατί η συναλλαγή με την πιστωτική κάρτα δεν έγινε αποδεκτή.

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

    pthread_mutex_lock(&mutex_seatA);
    bool flag_success_finding_seats=0;
	//bool flag_success_payment=0;
    int line=-1;
    if(possible_zone){//searching zone A
	   int i=0;  
		   while(i<NzoneA AND !flag_success_finding_seats){
		   struct Node* last;
		while (listA[i] != NULL) {
			if(listA[i]->next->data-listA[i]->data>choosen_seats){//βρήκαμε πιθανές θέσεις για τον πελάτη
				flag_success_finding_seats=1;
				pthread_mutex_lock(&mutex_cashier);
				while(cashier <= 0)
				{
				pthread_cond_wait(&cond_cashier ,&mutex_cashier);
				}
				cashier--;
					if(rand_r(&seed) % 100 / 100.0f <= Pcardsucces){//με επιτυχία πλήρωσε: Type A
						//flag_success_payment=1;
						sum_typeA++;
						
						pthread_mutex_lock(&mutex_balance);
						balance += choosen_seats * CzoneA;
					pthread_mutex_unlock(&mutex_balance);
					//book θέσεις
						j=listA[i]->data;
						sum=0
						while(j<listA[i]->next->data AND sum<choosen_seats ){
							zoneA[i,j]=customer_id;
							j++;
							sum++;
						}
						//update list with sum as starting node
					}
				
				else{//βρήκε θέσεις αλλά δεν μπόρεσε να πληρώσει: Type B
					sum_typeB++;
				}pthread_mutex_unlock(&mutex_cashier);
			}
			last = listA[i]->next;
			listA[i] = last->next;
			if(flag_success_finding_seats) break;

    	}
		pthread_mutex_unlock(&mutex_seatA);
		if(!flag_success_finding_seats) sum_typeC++;
	 	}
		 ++i;
	   }

else{//searching zone B
	pthread_mutex_lock(&mutex_seatB);
    int i=0;  
		   while(i<NzoneB AND !flag_success_finding_seats){
		   struct Node* last;
		while (listB[i] != NULL) {
			if(listB[i]->next->data-listB[i]->data>choosen_seats){//βρήκαμε πιθανές θέσεις για τον πελάτη
				flag_success_finding_seats=1;
				pthread_mutex_lock(&mutex_cashier);
				while(cashier <= 0)
				{
				pthread_cond_wait(&cond_cashier ,&mutex_cashier);
				}
				cashier--;
					if(rand_r(&seed) % 100 / 100.0f <= Pcardsucces){//με επιτυχία πλήρωσε: Type A
						//flag_success_payment=1;
						
						sum_typeA++;
						pthread_mutex_lock(&mutex_balance);
						balance += choosen_seats * CzoneA;
						pthread_mutex_unlock(&mutex_balance);
						//book θέσεις
						j=listB[i]->data;
						sum=0
						while(j<listB[i]->next->data AND sum<choosen_seats ){
							zoneB[i,j]=customer_id;
							j++;
							sum++;
						}
					}
				
				else{//βρήκε θέσεις αλλά δεν μπόρεσε να πληρώσει: Type B
					sum_typeB++;
				}
				pthread_mutex_unlock(&mutex_cashier);
			}
			last = listA[i]->next;
			listA[i] = last->next;
			if(flag_success_finding_seats) break;

    	}
		if(!flag_success_finding_seats) sum_typeC++;
	 	}
		 ++i;
	   }
	   pthread_mutex_lock(&mutex_seatB);
  
} 
    
    
    
    

int main(int argc, char *argv[]){
 if (argc != 3) {
		printf("ERROR: Το προγραμμα πρεπει να παιρνει 2 arguments(number of clients--> int, seed --> int)\n\n\n");
		exit(-1);
	}
	
	int customers = atoi(argv[1]);
   	if ( customers <= 0) {
		printf("ERROR: Ο αριθμός των πελατών πρεπει να ειναι ακεραιος θετικός\n");
		exit(-1);
	}

    sum_waiting_time = 0;
    sum_service_time = 0;
	telephonist = Ntel;
	seed = atoi(argv[2]);
	printf("\n \nStart simulation with %d customers \n \n \n", customers);
	

	//Initializing seats
	for(int i=0; i<NzoneA; ++i){
		push(*listA[i],0);
		push(*listA[i],10);
		for(int j=0;j<Nseat;++j)
		{
			zoneA[i,j] = -1;
		}
	}

	for(int i=0; i<NzoneB; ++i){
		push(*listB[i],0);
		push(*listB[i],10);
		for(int j=0;j<Nseat;++j)
		{
			zoneB[i,j] = -1;
		}
	}
	

	
	

	/*Create threads*/
	pthread_t id[customers];
	for(int i=0; i<customers; ++i)
	{
		pthread_create(&id[i], NULL, &execute, &i);
	}



	/*Wait all threads to finish*/
	for(int i=0; i<customers; ++i)
	{
		pthread_join(id[i], NULL);
	}

	/*Print results*/
	
	//if a seat is not booked the result will be -1


	printf("Πλάνο θέσεων: \n \n");
	for(int i=0; i<NzoneA; ++i){
		for (int j = 0; j < Nseat; j++) 
		{	if(zoneA[i,j] != -1)
			printf("Ζώνη Α / Σειρά %d / Θέση %d / Πελάτης %d\n", i, j, zoneA[i,j]);
			else
			printf("Ζώνη Α / Σειρά %d / Θέση %d / κανένας \n", i, j);
		}
	}

	for(int i=0; i<NzoneB; ++i){
		for (int j = 0; j < Nseat; j++) 
		{	if(zoneA[i,j] != -1)
			printf("Ζώνη B / Σειρά %d / Θέση %d / Πελάτης %d\n", i, j, zoneB[i,j]);
			else
			printf("Ζώνη B / Σειρά %d / Θέση %d / κανένας \n", i, j);
		}
	}

	printf("\n");
	printf("Τα συνολικά έσοδα είναι: %f € \n", balance);

	printf("Το ποσοστό των συναλλαγών όπου η κράτηση ολοκληρώθηκε επιτυχώς: %f \n", sum_typeA/customers);

	printf("Το ποσοστό των συναλλαγών όπου η κράτηση απέτυχε γιατί δεν υπάρχουν κατάλληλες θέσεις: %f \n", sum_typeB/customers);

	printf("Το ποσοστό των συναλλαγών όπου η κράτηση απέτυχε γιατί η συναλλαγή με την πιστωτική κάρτα δεν έγινε αποδεκτή: %f \n", sum_typeC/customers);
	
	printf("Ο μέσος χρόνος αναμονής είναι: %f \n", sum_waiting_time/customers);
	
	printf("Ο μέσος χρόνος εξυπηρέτησης είναι: %f \n", sum_service_time /customers);




	
	return 0;   
}
