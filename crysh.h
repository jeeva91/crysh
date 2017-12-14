#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sysexits.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<openssl/evp.h>
#include<openssl/aes.h>


int get_command(char*, size_t);
int execute(char*);
char* strstrip(char*);

int redirection(char* arg);
