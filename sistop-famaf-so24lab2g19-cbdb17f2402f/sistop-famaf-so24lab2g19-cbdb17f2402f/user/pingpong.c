#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXSEM 526

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "ERROR: usage: pingpong N\n");
    exit(1);
  }
  
  int pid;
  int sem0 = 0;
  int sem1 = 1;
  
  //if sem is used, found other unused
  while(sem_open(sem0, 0) == 0){
    sem0++;
    if(sem0 == MAXSEM){
      fprintf(2, "ERROR: sem_open failed\n");
      exit(1);
    }
  }
  while(sem_open(sem1, 1) == 0){
    sem1++;
    if(sem1 == MAXSEM){
      fprintf(2, "ERROR: sem_open failed\n");
      exit(1);
    }
  }
  int rally = atoi(argv[1]);
  if(rally <= 0){
    fprintf(2, "ERROR: Parameter must to be greater than 0\n");
    exit(1);
  }
  
  pid = fork();
  if(pid == -1){
    fprintf(2, "ERROR: fork failed\n");
    exit(1);
  }
  if(pid == 0){
    for(int i = 0; i < rally; i++){
      if(sem_down(sem0)==0){ 
        fprintf(2, "ERROR: sem_down failed\n");
        exit(1);
      }
      printf("    pong\n");
      if(sem_up(sem1)==0){
        fprintf(2, "ERROR: sem_up failed\n");
        exit(1);
      }
    }
    sem_close(sem0);
    sem_close(sem1);
  }else {
    for(int i = 0; i < rally; i++){
      if(sem_down(sem1)==0){
        fprintf(2, "ERROR: sem_down failed\n");
        exit(1);
      }
      printf("ping\n");
      if(sem_up(sem0)==0){
        fprintf(2, "ERROR: sem_up failed\n");
        exit(1);
      }
    }
    wait(0);
  } 
  exit(0);
}