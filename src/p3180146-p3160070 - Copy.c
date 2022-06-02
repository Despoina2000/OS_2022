
#include <stdio.h>
#include <stdlib.h>
#include "p3180146-p3160070.h"


//Variable

struct Node* listA[5];
struct Node* listB[5];

    
    
    

int main(int argc, char *argv[]){
	
	

	//Initializing seats
	for(int i=0; i<5; ++i){
		push(&listA[i],0);
		printList(&listA[i]);
		push(&listA[i],9);
		printList(&listA[i]);
	}

	for(int i=0; i<5; ++i){
		push(&listB[i],0);
		printList(&listB[i]);
		push(&listB[i],9);
		printList(&listB[i]);
	}
	
	return 0;   
}
