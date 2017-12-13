#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sysexits.h>
#include<sys/wait.h>

int get_command(char*, size_t);
int execute(char*);

int
main(void){
  char *lineptr = NULL;
  size_t linelen;


  getline(&lineptr, &linelen, stdin);
  if(linelen>0&&lineptr[linelen-1]=='\n'){
    lineptr[--linelen]= '\0';
    linelen-=1;
  }
  //  printf("the input is %s\n", lineptr);
  get_command(lineptr, linelen);
  //printf("after exec %s\n", lineptr);

  
  return 0;
}

int
get_command(char* commands, size_t len){
  char* token;
  if((commands==NULL)||(commands[0]=='\0'))
    return 0;
  token = strsep(&commands, ";");
  //printf("%s\n", token);
  if(token[0]!='\0')
  execute(token);
  get_command(commands, len-strlen(token)-1);
  return 0;
}

int
execute(char* command){
  if((command[0]=='\0')||command[0]=='\n')
    return 0;

  pid_t pid;
  int status;

  if((pid = fork())<0){
    perror("CRYsh unalbe to fork");
    
  }
  else if(pid==0){
    //chdir("/bin");
    execlp(command, command, (char*) 0);
    fprintf(stderr, "crysh: couldn't exex %s: %s\n", command, strerror(errno));
    exit(EX_DATAERR);
  }
  if((pid = waitpid( pid, &status, 0))<0)
    fprintf(stderr, "CRYsh: waitpid error: %s\n", strerror(errno));
  
  return 0;
}
