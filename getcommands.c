#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sysexits.h>
#include<sys/wait.h>

int get_command(char*, size_t);
int execute(char*);
char* strstrip(char* token);

int
main(void){
  char *commands, *lineptr = NULL;
  size_t linelen;

  linelen = getline(&lineptr, &linelen, stdin);
  commands = lineptr;
  if((commands = strstrip(commands))!=NULL)
    get_command(commands, linelen);
  free(lineptr);
  return 0;
}
/* gets the independent commands and executes them recursively */
int
get_command(char* commands, size_t len){
  char* token;
  
  if((commands==NULL)||(commands[0]=='\0'))
    return 0;

  token = strsep(&commands, ";");

  if((token = strstrip(token))!=NULL)
    execute(token);

  get_command(commands, len-strlen(token)-1);
  return 0;
}

/* function to strip out the space, tab and newline in front and back of string */
char*
strstrip(char* token){
  size_t  tokenlen;
  tokenlen = strlen(token);

  while((token[0]==' ')||(token[0]=='\t')||(token[0]=='\n'))
    token = token+1;

  tokenlen = strlen(token);
  while((token[tokenlen-1]=='\t')||(token[tokenlen-1]=='\n')||(token[tokenlen-1]==' ')){
    token[tokenlen-1]='\0';
    tokenlen-=1;
  }
  
  if(token[0]=='\0')
    token=NULL;
  return token;
}

/* forks are executes the given command */
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
