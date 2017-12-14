#include"crysh.h"
/*
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
  }*/
/* splits the commands with delimiter ; and executes them recursively */
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
  while((token[tokenlen-1]=='\t')||
	(token[tokenlen-1]=='\n')||
	(token[tokenlen-1]==' ')) {

    token[tokenlen-1]='\0';
    tokenlen-=1;
    
  }
  
  if(token[0]=='\0')
    token=NULL;
  return token;
}

/* forks and executes the given command */
int
execute(char* command){
  char **argv;
  if((command[0]=='\0')||command[0]=='\n')
    return 0;

  pid_t pid;
  int status;

  if((pid = fork())<0){
    perror("CRYsh unalbe to fork");
    
  }
  else if(pid==0){
    //chdir("/bin");
    int out_fd, err_fd;
    argv = parse_options(command, &out_fd, &err_fd);
    execvp(argv[0], argv);
    fprintf(stderr, "crysh: couldn't exex %s: %s\n",
	    command, strerror(errno));
    exit(EX_DATAERR);
  }
  if((pid = waitpid( pid, &status, 0))<0)
    fprintf(stderr, "CRYsh: waitpid error: %s\n", strerror(errno));
  
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
