#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "p3180146-p3160070.h"

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
int zoneA[ NzoneA][ Nseat];
int zoneB[ NzoneB][ Nseat];
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
int rc;

    struct timespec timer_on;
	struct timespec	service_timer;
	struct timespec timer_off;
    int id=*(int *)customer_id;

    clock_gettime(CLOCK_REALTIME, &timer_on); //Start timer
    rc= pthread_mutex_lock(&mutex_telephonist);
	assert_successful_mutex_action(rc);
    while(telephonist <= 0)
	{
	pthread_cond_wait(&cond_telephonist ,&mutex_telephonist);
	}
	--telephonist;
	clock_gettime(CLOCK_REALTIME, &service_timer);
    rc=pthread_mutex_unlock(&mutex_telephonist);

    int choosen_seats = rand_r(&seed) % Nseathigh + Nseatlow;
	int respond_time = rand_r(&seed) % tseathigh + tseatlow;
    int possible_zone= rand_r(&seed) % 100 / 100.0f;
	sleep(respond_time);

    
    int flag_success_finding_seats=0;
	//bool flag_success_payment=0;
    int line=-1;
    if(possible_zone <= PzoneB){//searching zone A
	   int i=0; 
	   rc= pthread_mutex_lock(&mutex_seatA);
	   assert_successful_mutex_action(rc); 
		   while(i<NzoneA && !flag_success_finding_seats){
		   struct Node* _list= listA[i];
		while ( _list != NULL) {
			if((_list->next->data-_list->data)+1>=choosen_seats){//βρήκαμε πιθανές θέσεις για τον πελάτη
				flag_success_finding_seats=1;
				rc= pthread_mutex_lock(&mutex_cashier);
				assert_successful_mutex_action(rc);
				while(cashier <= 0)
				{
				pthread_cond_wait(&cond_cashier ,&mutex_cashier);
				}
				cashier--;
				rc= pthread_mutex_unlock(&mutex_cashier);
				assert_successful_mutex_action(rc);
					if(rand_r(&seed) % 100 / 100.0f <= Pcardsucces){//με επιτυχία πλήρωσε: Type A
						//flag_success_payment=1;
						sum_typeA++;
						
						rc= pthread_mutex_lock(&mutex_balance);
						assert_successful_mutex_action(rc);
						balance += choosen_seats * CzoneA;
						rc= pthread_mutex_unlock(&mutex_balance);
						assert_successful_mutex_action(rc);
					//book θέσεις
						int j=_list->data;
						int sum=0;
						while(j<=_list->next->data && sum<choosen_seats ){
							zoneA[i][j]=id;
							j++;
							sum++;
						}
						//update list with j 
						if((_list->next->data-_list->data)+1==choosen_seats) deleteNodebyKey(&listA[i], j-1);
						else{
						push(&listA[i], j);}
						deleteNodebyKey(&listA[i], _list->data);
						
					}
					
				
				else{//βρήκε θέσεις αλλά δεν μπόρεσε να πληρώσει: Type C
					sum_typeC++;
				}
				rc= pthread_mutex_lock(&mutex_cashier);
				assert_successful_mutex_action(rc); 
				cashier++;
				pthread_cond_signal(&cond_cashier);
				rc= pthread_mutex_unlock(&mutex_cashier);
				assert_successful_mutex_action(rc);
			}
			_list = _list->next->next;
			if(flag_success_finding_seats) break;

    	}
		rc= pthread_mutex_unlock(&mutex_seatA);
		if(!flag_success_finding_seats) sum_typeB++;
		++i;
	 	}
		 
	   }

else{//searching zone B
	rc= pthread_mutex_lock(&mutex_seatB);
	assert_successful_mutex_action(rc);
    int i=0;  
		   while(i<NzoneB && !flag_success_finding_seats){
		   struct Node* _list=listB[i];
		while (_list != NULL) {
			if((_list->next->data-_list->data)+1>=choosen_seats){//βρήκαμε πιθανές θέσεις για τον πελάτη
				flag_success_finding_seats=1;
				rc= pthread_mutex_lock(&mutex_cashier);
				assert_successful_mutex_action(rc);
				while(cashier <= 0)
				{
				pthread_cond_wait(&cond_cashier ,&mutex_cashier);
				}
				cashier--;
				rc= pthread_mutex_unlock(&mutex_cashier);
				assert_successful_mutex_action(rc);
					if(rand_r(&seed) % 100 / 100.0f <= Pcardsucces){//με επιτυχία πλήρωσε: Type A
						//flag_success_payment=1;
						
						sum_typeA++;
						rc= pthread_mutex_lock(&mutex_balance);
						assert_successful_mutex_action(rc);
						balance += choosen_seats * CzoneB;
						rc= pthread_mutex_unlock(&mutex_balance);
						assert_successful_mutex_action(rc);
						//book θέσεις
						int j=_list->data;
						int sum=0;
						while(j<=_list->next->data && sum<choosen_seats ){
							zoneB[i][j]=id;
							j++;
							sum++;
						}
						//update list with j 
						if((_list->next->data-_list->data)+1==choosen_seats) deleteNodebyKey(&listB[i], j-1);
						else{
						push(&listB[i], j);}
						deleteNodebyKey(&listB[i], _list->data);
					}
				
				else{//βρήκε θέσεις αλλά δεν μπόρεσε να πληρώσει: Type C
					sum_typeC++;
				}
				rc= pthread_mutex_lock(&mutex_cashier);
				assert_successful_mutex_action(rc);
				cashier++;
				pthread_cond_signal(&cond_cashier);
				rc= pthread_mutex_unlock(&mutex_cashier);
				assert_successful_mutex_action(rc);
			}
			_list = _list->next->next;
			if(flag_success_finding_seats) break;

    	}
		if(!flag_success_finding_seats) sum_typeB++;
		++i;
	 	}
		 
	   }
	   rc= pthread_mutex_unlock(&mutex_seatB);
	   assert_successful_mutex_action(rc);
		rc= pthread_mutex_lock(&mutex_telephonist);
	assert_successful_mutex_action(rc);
	   telephonist++;
	   rc= pthread_mutex_unlock(&mutex_telephonist);
	assert_successful_mutex_action(rc);
	   pthread_cond_signal(&cond_telephonist);
	   pthread_exit(NULL);
  
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

	balance = 0;
    sum_waiting_time = 0;
    sum_service_time = 0;
	telephonist = Ntel;
	seed = atoi(argv[2]);
	printf("\n \nStart simulation with %d customers \n \n \n", customers);
	

	//Initializing seats
	for(int i=0; i<NzoneA; ++i){
		push(&listA[i],0);
		append(&listA[i],9);
		for(int j=0;j<Nseat;++j)
		{
			zoneA[i][j] = -1;
		}
	}

	for(int i=0; i<NzoneB; ++i){
		push(&listB[i],0);
		append(&listB[i],9);
		for(int j=0;j<Nseat;++j)
		{
			zoneB[i][j] = -1;
		}
	}
	
	/*Create threads*/
	int rc;
	pthread_mutex_init(&mutex_telephonist, NULL);
	pthread_mutex_init(&mutex_cashier, NULL);
	pthread_mutex_init(&mutex_seatA, NULL);
	pthread_mutex_init(&mutex_seatB, NULL);
	pthread_mutex_init(&mutex_listA, NULL);
	pthread_mutex_init(&mutex_listB, NULL);
	pthread_mutex_init(&mutex_printer, NULL);
	pthread_mutex_init(&mutex_balance, NULL);
	pthread_cond_init(&cond_telephonist, NULL);
	pthread_cond_init(&cond_cashier, NULL);
   
	pthread_t id[customers];
	for(int i=0; i<customers; ++i)
	{
		printf("In main: creating thread %ld\n", i);
		rc=pthread_create(&id[i], NULL, processing, &i);
		if (rc){
				printf("ERROR code from pthread_create() is %d\n", rc);
      	exit(-1);
    	}
	}

	/*Wait all threads to finish*/
	for(int i=0; i<customers; ++i)
	{
		pthread_join(id[i], NULL);
	}
	pthread_mutex_destroy(&mutex_telephonist);
	pthread_mutex_destroy(&mutex_cashier);
	pthread_mutex_destroy(&mutex_seatB);
	pthread_mutex_destroy(&mutex_listA);
	pthread_mutex_destroy(&mutex_listB);
	pthread_mutex_destroy(&mutex_printer);
	pthread_mutex_destroy(&mutex_balance);
	pthread_cond_destroy(&cond_telephonist);
	pthread_cond_destroy(&cond_cashier);

	/*Print results*/
	
	//if a seat is not booked the result will be -1


	printf("Πλάνο θέσεων: \n \n");
	for(int i=0; i<NzoneA; ++i){
		for (int j = 0; j < Nseat; j++) 
		{	if(zoneA[i][j] != -1)
			printf("Ζώνη Α / Σειρά %d / Θέση %d / Πελάτης %d\n", i, j, zoneA[i][j]);
			else
			printf("Ζώνη Α / Σειρά %d / Θέση %d / κανένας \n", i, j);
		}
	}

	for(int i=0; i<NzoneB; ++i){
		for (int j = 0; j < Nseat; j++) 
		{	if(zoneB[i][j] != -1)
			printf("Ζώνη B / Σειρά %d / Θέση %d / Πελάτης %d\n", i, j, zoneB[i][j]);
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

void assert_successful_mutex_action(int response_code)
{
    if (response_code != 0) 
    {    
        printf("Σφάλμα στο κλείδωμα/ξεκλείδωμα του mutex με κωδικο σφαλματος %d\n", response_code);
        pthread_exit(&response_code);
    }
}
