//executable file: table.c, phil
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

void main(int argc, char *argv[]){

  int ret, phils, forks, value, phi, waitTime, index, rem, rem1, rem2;
  long forkNums, philNums;

  if(argc != 2){
	perror("Too many arguments");
	exit(-1);
  }

  phi = atoi(argv[1]);
  if(phi < 0){
	perror("Silly that's too small");
	exit(-1);
  }
  if(phi > 4){
	perror("Honey, your table is only so big");
	exit(-1);
  }
  forkNums = 5;
  philNums = 10;
  value = 1;
  index = 0;

  srand(3*phi); //change to be function of phi

  forks = semget(forkNums, 5, IPC_CREAT | 0600); //returns < 0 on error
	if(forks < 0){
	  perror ("Error with forks sem creation\n");
	  exit(-1);
        }
	else{
	  printf("Forks sem is: %d\n", forks);
	  fflush(stdout);
	}
  phils = semget(philNums, 1, IPC_CREAT | 0600); 
	if(phils < 0)
	  perror ("Error with phils sem creation\n");
	else{
	  printf("Phils sem is: %d\n", forks);
	  fflush(stdout);
	}
  struct sembuf waitL,waitR, signalL, signalR, alive; //two variables holding structures for semop()

  waitL.sem_flg = 0;    signalL.sem_flg = 0; 
  waitL.sem_num = phi;  signalL.sem_num = phi; 
  waitL.sem_op = -1;    signalL.sem_op =1; 

  waitR.sem_flg = 0;        signalR.sem_flg = 0; 
  waitR.sem_num = phi+1%5;  signalR.sem_num = phi+1%5; 
  waitR.sem_op = -1;        signalR.sem_op =1;  

  alive.sem_flg = 0;
  alive.sem_num = 0;
  alive.sem_op = -1;
//initialize forks/////////////////////////////////////////////////////////////
  ret = semctl(forks, 0, SETVAL, value);
  if(ret < 0){
	perror("No fork, try spoon?");
	exit(-1);
  } 
  ret = semctl(forks, 1, SETVAL, value);
  if(ret < 0){
	perror("No fork, try spoon?");
	exit(-1);
  }
  ret = semctl(forks, 2, SETVAL, value);
  if(ret < 0){
	perror("No fork, try spoon?");
	exit(-1);
  } 
  ret = semctl(forks, 3, SETVAL, value);
  if(ret < 0){
	perror("No fork, try spoon?");
	exit(-1);
  }
  ret = semctl(forks, 4, SETVAL, value);
  if(ret < 0){
	perror("No fork, try spoon?");
	exit(-1);
  }
//initialize philosophers//////////////////////////////////////////////////////
  ret = semctl(phils, 0, SETVAL, forkNums); 
  if(ret < 0){
	perror("No phils showed up, try making something good for dinner?");
	exit(-1);
  }
 
  waitTime = rand()%31+60;
  printf("Philosopher %d is in and going to sleep for %d\n", phi, waitTime);
  fflush(stdout);
  sleep(waitTime); //initialize philosophers
  for(index=0;index<3;index++){
		waitTime = rand()%11+10;
		printf("Philosopher %d is in and going to think for %d\n", phi, waitTime);
		fflush(stdout);
	  sleep(waitTime);  //think between 10 and 20
//try to pick up forks/////////////////////////////////////////////////////////
	  ret = semop(forks, &waitL, 1);
		printf("Philosopher %d picked up left fork\n", phi);
		fflush(stdout);
	  ret = semop(forks, &waitR, 1);
		waitTime = rand()%6+5;
		printf("Philosopher %d picked up right fork and will eat for %d\n", phi, waitTime);
		fflush(stdout);
	
	    sleep(waitTime);  //eat between 5 and 10

	  ret = semop(forks, &signalR, 1);
		printf("Philosopher %d put down right fork\n", phi);
		fflush(stdout);
	  ret = semop(forks, &signalL, 1);
	  	printf("Philosopher %d put down left fork\n", phi);
		fflush(stdout);
  }
  printf("Philosopher %d is done\n", phi);
  fflush(stdout);
  if(rem == 1){
	printf("In if\n");
	fflush(stdout);
	ret = semctl(forks, 5, IPC_RMID, (void*)0);
	if(ret < 0){
	  perror("Couldn't remove forks \n");
	  exit(-1);
	}
	else{
	  printf("Forks gone\n");
 	  fflush(stdout);
	}
	ret = semctl(phils, 1, IPC_RMID, (void*)0);
	if(ret < 0){
	  perror("Couldn't remove phils\n");
	  exit(-1);
	}
	else{
	  printf("Phils gone\n");
 	  fflush(stdout);
	}
  }
  else{
	  //printf("In else\n");
	  //fflush(stdout);
	  rem = semop(phils, &alive, 1);
	  //printf("Phils decremented\n");
	  //fflush(stdout);
	  rem = semctl(phils, 0, GETVAL, (void*)0);
	  //printf("rem is %d \n", rem);
	  //fflush(stdout);
  }
  exit(0);
}//end main



