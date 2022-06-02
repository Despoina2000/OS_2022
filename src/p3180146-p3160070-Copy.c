
#include <stdio.h>
#include <stdlib.h>
#include "p3180146-p3160070.h"


//Variable

struct Node* listA[5];


    
    
    

int main(int argc, char *argv[]){
	
	

	//Initializing 
	for (int i = 0; i < NzoneA; ++i) {
		push(&listA[i], 2);
		append(&listA[i], 4);
		append(&listA[i], 7);
		append(&listA[i], 9);
	}
	int j = 2;
	int sum = 0;
	while (j <= 4 && sum < 3) {
		j++;
		sum++;
	}
	//update list with j 
	if ((4 - 2) + 1 == 3) deleteNodebyKey(&listA[0], j - 1);
	else{push(&listA[0], j);
	printList(listA[0]);
	sortList(listA[0]);
	printList(listA[0]);}
	deleteNodebyKey(&listA[0], 2);
	printList(listA[0]);
	

	
	return 0;   
}
