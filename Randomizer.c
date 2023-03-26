/*
*  PLAGIARISM STATEMENT
*    I certify that this assignment/report is my own work, based on my personal
*    study and/or research and that I have acknowledged all material and sources
*    used in its preparation, whether they be books, articles, reports, lecture notes, online sources
*    and any other kind of document, electronic or personal communication. I also
*    certify that this assignment/report has not previously been submitted for
*    assessment in any other unit, except where specific permission has been granted
*    from all unit coordinators involved, or at any other time in this unit, and that I
*    have not copied in part or whole or otherwise plagiarised the work of other
*    students and/or persons.
*
*  Name       :  Andrew Darby
*  PSU Email  :  ald149@psu.edu
*  Assignment :  lab07
*/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t FULL_SLOTS;                           // init semaphore for full spaces on stack
sem_t EMPTY_SLOTS;                          // init semaphore for empty spaces on stack
pthread_mutex_t LOCK;                 // will protect the stack from being double accessed
const int STACK_SIZE = 5;                   // store const variable for stack size
int stack[] = {-1, -1, -1, -1, -1};   // init stack with all set to -1
int i = 0;                              // index to keep place in stack
int count = 0;


void * randFunction(void *arg);
void * consFunction( void *arg);



int main()
{
    
	int gen;
	int gen2;
	void *ret;

    // init mutex for locking share resource(stack)
	gen = pthread_mutex_init(&LOCK, NULL);
    if (gen != 0) 
    	{
        	perror("Mutex initialization failed");
        	exit(EXIT_FAILURE);
    	}
    	
	pthread_t producer;
	pthread_t consumer;               // init pthreads 
	
	// init FULL_SLOTS sem
	sem_init(&FULL_SLOTS, 0, 0);

    // init EMPTY_SLOTS sem
    sem_init(&EMPTY_SLOTS, 0, 5);
	
   
    // Create thread to generate random #'s' 
    pthread_create(&producer, NULL, randFunction, NULL);
    
	  // Create thread to consume/print/remove numbers
    pthread_create(&consumer, NULL, consFunction, NULL);
	
	pthread_join(producer, &ret);
	pthread_join(consumer, &ret);
	
	return 0;
}
	
	
void * randFunction(void *arg)
{
	
	srand(time(0));
	
	while(1)
	{
		sem_wait(&EMPTY_SLOTS);         // wait if no empty slots
	    pthread_mutex_lock(&LOCK);      // lock resource
	    stack[i] = rand() % 100 + 1;        // generate random number and add to stack then print
	    printf("Randomizer Thread >> RAND Generated %d << Done.\n", stack[i]);
	    i++;
	    pthread_mutex_unlock(&LOCK);    // release resource
	    sem_post(&FULL_SLOTS);
	    
		count++;
	    if (count == 15) 
			break;
	    
	} // end while loop
	printf("Done Producing.\n");
	pthread_exit("Done Producing.");
} // ennd randFunction

void * consFunction( void *arg)
{
	while(1)
	{
	    sem_wait(&FULL_SLOTS);
	    pthread_mutex_lock(&LOCK);      // lock resource
	    printf("CONSUME Thread >> RAND Found %d << Done.\n", stack[i -1]);
	    stack[i] = -1;
	    i--;
	    pthread_mutex_unlock(&LOCK);    // release resource
	    sem_post(&EMPTY_SLOTS);
	    //i--;
	    
	   if(count == 14)
		break;
	}
	printf("Finished Consuming.\n");
	pthread_exit("Done Consuming.");
} // end method consFunction