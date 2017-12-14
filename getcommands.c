#include"crysh.h"
#include<fcntl.h>
char** get_parse_options(char* input, int* argc);
int redirection(char* arg);
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
  int argc;
  int fd;
  
  if((pid = fork())<0){
    perror("CRYsh unalbe to fork");
    
  }
  else if(pid>0){
    //chdir("/bin");
    //int out_fd, err_fd;

    argv = get_parse_options(command, &argc);
    if((fd=redirection(argv[argc]))==0){
      argv[argc]='\0';
    }
    else if((fd=redirection(argv[argc-1]))== 0){
      argv[argc-1]='\0';
      }
    
    execvp(argv[0], argv);
    fprintf(stderr, "crysh: couldn't exex %s: %s\n",
	    command, strerror(errno));
    exit(EX_DATAERR);
  }
  if((pid = waitpid( pid, &status, 0))<0)
    fprintf(stderr, "CRYsh: waitpid error: %s \n", strerror(errno));
  
  return 0;
}

int
redirection(char* arg){
  int fd;
  char *file;
  if((strncmp(arg, "2>>",3))==0){
    /* perform stderr redirection with append to file */
    file = strsep(&arg, "2>>");
    fd = open(arg, O_APPEND|O_CREAT|O_WRONLY);
    if(fd==-1){
      fprintf(stderr,"unable to open the file: %s error: %s\n", arg, strerror(errno));
      exit(128);
    }
    dup2(fd, STDERR_FILENO);
    return fd;
  }
  else if((strncmp(arg, ">>",2))==0){
    /* perform stdout redirection with append to file */
    file = strsep(&arg, ">>");
    fd = open(arg, O_APPEND|O_CREAT|O_WRONLY);
    if(fd==-1){
      fprintf(stderr,"unable to open the file: %s error: %s\n", arg, strerror(errno));
      exit(128);
    }
    dup2(fd, STDOUT_FILENO);
    return fd;
  }
  else if((strncmp(arg, "2>", 2))==0){
    /*redirect stderr to file*/
    file = strsep(&arg, "2>");
    fd = open(arg, O_CREAT|O_WRONLY);
    if(fd==-1){
      fprintf(stderr,"unable to open the file: %s error: %s\n", arg, strerror(errno));
      exit(128);
    }
    dup2(fd, STDERR_FILENO);
    return fd;
  }
  else if((strncmp(arg, ">", 1))==0){
    file = strsep(&arg, ">");
    fd = open(arg, O_CREAT|O_WRONLY);
    if(fd==-1){
      fprintf(stderr,"unable to open the file: %s error: %s\n", arg, strerror(errno));
      exit(128);
    }
    dup2(fd, STDIN_FILENO);
    return fd;
  }
  return 0;
}


char** get_parse_options(char* input, int* argc){
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

  *argc = i-1;

  return argv;
}

