#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#define INIT_MUTEX 2 
#define INIT_EMPTY 2 
#define INIT_FULL  2 
#define EMPTY_ARG  0
const int PROD = 0;
const int CONS = 1;
#define SEMKEY 0

typedef struct sembuf sem_struct;

int create_semaphore(int);
void criticalSection(int);
void set_sem_struct(sem_struct[],int);
void set_sem_values(int,int,int);
void consumer(int, int, int, sem_struct[], sem_struct[]);
void producer(int, int, int, sem_struct[], sem_struct[]);

int main(int argc, char* argv[])
{
  /*
  short int out[1];
  short int in[1];
  out[0]; 
  in[0] = 11; 
	sem_struct wait[1], signal[1]; //create semaphore structs
 	int mutex, empty, full;   //will hold semaphore identifiers 
 	int value, status, ch_stat, i;

 	set_sem_struct(wait,-1);
 	set_sem_struct(signal,1);

	//mutex = create_semaphore(1);
 	//empty = create_semaphore(100);
 	//full = create_semaphore(0);
  mutex = semget(SEMKEY, 1, 0777 | IPC_CREAT);
	empty = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  full = semget(SEMKEY, 1, 0777 | IPC_CREAT);

  semctl(mutex, 1 , SETALL, in);
  //semctl(empty, 1 , SETALL, 100);
  //semctl(full, 1 , SETALL, 0);

	//mutex = 1;
	//empty = 100;
	//full = 0;
  
  	//set_sem_values(mutex, empty, full);
    
  semctl(empty, 1, GETALL, out);
    printf("Main test: %d\n", out[0]);

  printf("Initial semaphore values: mutex = %d, empty = %d, full = %d\n", mutex, empty, full);
 
 if (fork() == 0)
 {
   producer(mutex, empty, full, wait, signal);
 }
 else
   consumer(mutex, empty, full, wait, signal);
 
 
  printf("Final semaphore values: mutex = %d, empty = %d, full = %d\n", mutex, empty, full);
  */
  int semid;
 short int in[1];
 short int out[1];

 in[0] = 1; 
 out[0]; 

  //sem_struct wait[1], signal[1]; //create semaphore structs

  sem_struct sem_wait[1], sem_signal[1];
//set the buffer values for wait.
 sem_wait[0].sem_num = 0;
 sem_wait[0].sem_op = -1; //decrement
 sem_wait[0].sem_flg = SEM_UNDO;   
 //set the buffer values for signal 
 sem_signal[0].sem_num = 0;
 sem_signal[0].sem_op = 1; //increment
 sem_signal[0].sem_flg = SEM_UNDO; 

  int mutex, empty, full; 
 
 //create a semaphore set of 1  semaphore
  mutex = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  empty = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  full = semget(SEMKEY, 1, 0777 | IPC_CREAT);
 //initialize the first semaphore in the set identified by semid to the value in the input buffer,in
 semctl(mutex, 1 , SETALL, in);
 in[0] = 100;
 semctl(empty, 1 , SETALL, in);
 in[0] = 0;
 semctl(full, 1 , SETALL, in);

 //write the value contained in the first semaphore of the semaphore set to the output buffer,out. 
 printf("Initial semaphore values\n");
 semctl(mutex, 1, GETALL, out);
 printf("mutex: %d\n", out[0]);
 semctl(empty, 1, GETALL, out);
 printf("empty: %d\n", out[0]);
 semctl(full, 1, GETALL, out);
 printf("full: %d\n", out[0]);

 //printf("Initial semaphore values: mutex = %d, empty = %d, full = %d\n", mutex, empty, full);
 
 if (fork() == 0)
 {
   producer(mutex, empty, full, sem_wait, sem_signal);
 }
 else
   consumer(mutex, empty, full, sem_wait, sem_signal);
 

 printf("final semaphore values\n");
 semctl(mutex, 1, GETALL, out);
 printf("mutex: %d\n", out[0]);
 semctl(empty, 1, GETALL, out);
 printf("empty: %d\n", out[0]);
 semctl(full, 1, GETALL, out);
 printf("full: %d\n", out[0]);
  //printf("Final semaphore values: mutex = %d, empty = %d, full = %d\n", mutex, empty, full);
 semctl(mutex, 1, IPC_RMID, NULL);
 semctl(empty, 1, IPC_RMID, NULL);
 semctl(full, 1, IPC_RMID, NULL);
 
}

int create_semaphore(int key)
{
 int new_sem;
 if ((new_sem = semget(key, 1, 0777 | IPC_CREAT)) == -1)
   {
    perror("semget failed");
    exit(1);
   }
 return new_sem;
}
 
 void set_sem_struct(sem_struct sem[], int op_val)
{
 sem[0].sem_num = 0;
 sem[0].sem_op = op_val;
 sem[0].sem_flg = SEM_UNDO;
} 

void set_sem_values(int mutex, int empty, int full)
{
 semctl(mutex, 0, SETVAL, INIT_MUTEX);
 semctl(empty, 0, SETVAL, INIT_EMPTY);
 semctl(full, 0, SETVAL, INIT_FULL);
}

void producer(int mutex, int empty, int full, sem_struct wait[], sem_struct signal[])
{
  short int out[1];
  for(int i = 0; i < 5; i++)
   {
    
   // semctl(empty, 1, GETALL, out);
 //printf("empty before: %d\n", out[0]);
     semop(empty, wait, 1);
//semctl(empty, 1, GETALL, out);
 //printf("empty after: %d\n", out[0]);
     //mutex--;
     semop(mutex, wait, 1);
     criticalSection(PROD);
     //mutex++;
     semop(mutex, signal, 1);
     //full++; //experiment with putting delays after this line
     semop(full, signal, 1);
    for (int i = 0; i < 80000; i++)
    {
      int r;
      r = 9;
      r++;
    }
   }
}
 
void consumer(int mutex, int empty, int full, sem_struct wait[], sem_struct signal[])
{
  short int out[1];
  for (int i = 0; i <5; i++)
   {
    //semctl(full, 1, GETALL, out);
    //printf("full before: %d\n", out[0]);
     //full--;
     semop(full, wait, 1);
     //semctl(full, 1, GETALL, out);
    //printf("full after: %d\n", out[0]);
      //mutex--;
      semop(mutex, wait, 1);
       criticalSection(CONS);
        //mutex++;
        semop(mutex, signal, 1);
        //empty++; //experiment with putting delays after this line
        semop(empty, signal, 1);
   }
}

void criticalSection(int who)
{
  if (who == PROD)
    printf("Producer making an item\n");
 else
    printf("Consumer consuming an item\n");
 }
