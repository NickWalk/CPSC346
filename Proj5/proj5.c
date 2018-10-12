/*
Class: CPSC 346-01
Team Member 1: Nicholas Walker
Team Member 2: N/A
GU Username of project lead: nwalker
Pgm Name: proj5.c
Pgm Desc: Peterson Algorithm
Usage: ./a.out
*/
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parent(int, int, int*);
void child(int, int, int*);
void cs(char, int, int*);
void non_cs(int);

void main(int argc, char* argv)
{
 char*  address;
 int    shmid;
 int time_child = 5;
 int time_parent = 5;
 int time_parent_non_cs = 2;
 int time_child_non_cs = 2;

 if ((argc != 5) && (argc != 1))
  {
   fprintf(stderr, "Error\n"); 
   exit(EXIT_FAILURE);
  }
  if(argc != 1)
  {
    int time_child = argv[1];
    int time_parent = argv[2];
    int time_parent_non_cs = argv[3];
    int time_child_non_cs = argv[4];
  }
 int* turn;
 int* pr0;
 int* pr1;
 shmid = shmget(0,3,0777 | IPC_CREAT);
 turn = (int*)shmat(shmid,0,0); 
 pr0 = (int*)turn + 1;
 pr1 = (int*)pr0 + 1;
 *pr0 = 0;
 *pr1 = 0;

 //fork here
 if (fork() == 0)
    child(time_child, time_child_non_cs, turn);
 else 
    parent(time_parent, time_parent_non_cs, turn);
 //parBegin
 //{
  //child(time_child, time_child_non_cs);
  //parent(time_parent, time_parent_non_cs);
 //}
 //parEnd
}

void parent(int time_crit_sect, int time_non_crit_sect, int* shared)
{
  //fprintf(stderr, "parent ran\n"); 
 for (int i = 0; i < 10; i++)
  {
   *(shared+1)= 1; //pr0
   *shared = 1; // turn
   while((*(shared+2) == 1) && ((*shared) == 1))
   {
   //printf("before: turn = %i\n", *shared);
   //printf("before: pr0 = %i\n", *(shared+1));
   //printf("before: pr1 = %i\n", *(shared+2));
   
    cs('p', time_crit_sect, shared);
    *(shared+1) = 0;
    //printf("after: turn = %i\n", *shared);
   //printf("after: pr0 = %i\n", *(shared+1));
   //printf("after: pr1 = %i\n", *(shared+2));
   }
    
   non_cs(time_non_crit_sect); 
  }
}

void child(int time_crit_sect, int time_non_crit_sect, int* shared)
{
  // fprintf(stderr, "child ran\n"); 
 for (int i = 0; i < 10; i++)
  {
   *(shared+2) = 1; //pr1
   //fprintf(stderr, "pr1 just changed to 1!!!!!!!!!!!\n");
   *shared = 0; //turn
   while((*(shared+1) == 1) && ((*shared) == 0))
   {
   	//printf("before: turn = %i\n", *shared);
  // printf("before: pr0 = %i\n", *(shared+1));
   //printf("before: pr1 = %i\n", *(shared+2));
    //fprintf(stderr, "ran2\n"); 
   
    cs('c', time_crit_sect, shared);
    *(shared+2) = 0;
    // fprintf(stderr, "pr1 just changed to 0!!!!!!!!!!!\n");
    //printf("after: turn = %i\n", *shared);
   //printf("after: pr0 = %i\n", *(shared+1));
   //printf("after: pr1 = %i\n", *(shared+2));
   }

   non_cs(time_non_crit_sect); 
  }
}

void cs(char process, int time_crit_sect, int* shared)
{
 if (process == 'p')
  {
   printf("parent in critical sction\n");
   *(shared+2) = 0;
   sleep(time_crit_sect);
   printf("parent leaving critical section\n");
  }
 else
  {
   printf("child in critical sction\n");
   *(shared+1) = 0;
   sleep(time_crit_sect);
   printf("child leaving critical section\n");
  }
}

void non_cs(int time_non_crit_sect)
{
 sleep(time_non_crit_sect);
}
