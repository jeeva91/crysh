#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

char**
parse_options(char*, int*, int*);

int
main(void){
  char** arg;
  int out_fd, err_fd;
  char command[]="ls \t\t  -l -A";
  arg = parse_options(command, &out_fd, &err_fd);
  execvp(arg[0], arg);


  return 0;
}
char** 
parse_options(char* input, int* out_fd, int *err_fd){
  int len = strlen(input);
  char** argv;
  char* token;
  int i;
  
  if((argv=malloc(len))==NULL){
    fprintf(stderr, "malloc failed in %s\n", input);
  }

  i=0;
  token = strtok(input," \t");
  do{

    argv[i] = token;
    token = strtok(NULL," \t");
    i+=1;
  }while(token!=NULL);
  

  return argv;
}
