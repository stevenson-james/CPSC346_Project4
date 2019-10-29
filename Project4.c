/**

 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE				10
#define NUMBER_OF_THREADS	3

void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */

int list[SIZE] = {7,12,19,3,18,4,2,6,15,8};

int result[SIZE];

typedef struct
{
	int from_index;
	int to_index;
} parameters;

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main (int argc, const char * argv[]) 
{
	int i;
    
	pthread_t workers[NUMBER_OF_THREADS];
	
	printf("Unsorted array is: ");
	for (int i = 0; i < SIZE; i++) {
		printf("%d : %d", i, list[i]);
	}
	printf("\n");

	/* establish the first sorting thread */
	//1. call malloc to allocate a “parameters”
	parameters *threadParam = (parameters *) malloc (sizeof(parameters));
	//2. use “parameters” to specify the first half of the array
	threadParam->from_index = 0;
	threadParam->to_index = (SIZE/2) - 1;
      //3. create the first thread
	pthread_create(&workers[0], 0, sorter, threadParam);

	/* establish the second sorting thread */
	//1. call malloc to allocate a “parameters”
	threadParam = (parameters *) malloc (sizeof(parameters));
	//2. use “parameters” to specify the second half of the array
	threadParam->from_index = SIZE/2;
	threadParam->to_index = SIZE - 1;
    //3. create the second thread
	pthread_create(&workers[1], 0, sorter, threadParam);
	
	/* now wait for the 2 sorting threads to finish */
	// use ptheread_join; wait for 2 sorting threads to finish 
	ptheread_join(workers[0],NULL);
	ptheread_join(workers[1],NULL);

	/* establish the merge thread */
	//reuse “parameters” to hold the beginning index in each half
	//check if unnecessary
	threadParam = (parameters *) malloc (sizeof(parameters));
	//from_index is start of first, to_index is start of second
	threadParam->from_index = 0;
	threadParam->to_index = SIZE/2;
	//create the third thread: merge 
	pthread_create(&workers[2], 0, merger, threadParam);
	
	/* wait for the merge thread to finish */
	 pthread_join(workers[2], NULL);

	/* output the sorted array */
	printf("Sorted array is: ");
	for (int i = 0; i < SIZE; i++) {
		printf("%d : %d", i, result[i]);
	}
	printf("\n");

    return 0;
}

/**
 * Sorting thread.
 *
 * This thread can essentially use any algorithm for sorting
 */

void *sorter(void *params)
{
	parameters* p = (parameters *)params;
	int start = p->from_index;
	int pSize = p->to_index + 1;
	
	// Sorting algorithm here. It can be any sorting algorithm.
	for (int i = start; i < pSize - 1; i++) {
		int min = i;
		for (int j = start; i < start; j++) {
			if(list[j] < list[min])
				min = j;
		}
		swap(&list[min], &list[i]);
	}
	
	pthread_exit(0);
}

/**
 * Merge thread
 *
 * Uses simple merge sort for merging two sublists
 */

void *merger(void *params)
{
	parameters* p = (parameters *)params;
	
	//reuse “parameters” to hold the first index in each half
	//merge two sorted sublist to the arry result
	int firstArr = p->from_index;
	int secondArr = p->to_index;
	int i = 0;
	while (firstArr < p->to_index && secondArr < SIZE) {
		if (list[firstArr] < list[secondArr]) {
			result[i] = list[firstArr];
			++firstArr;
		}
		else {
			result[i] = list[secondArr];
			++secondArr;
		}
		++i;
	}
	while (firstArr < p->to_index) {
		result[i] = list[firstArr];
		++firstArr;
		++i;
	}

	while (secondArr < SIZE) {
		result[i] = list[secondArr];
		++secondArr;
		++i;
	}

	pthread_exit(0);
}