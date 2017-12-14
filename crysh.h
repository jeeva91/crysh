#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sysexits.h>
#include<sys/wait.h>

#define OUT_N_ERR 1
#define OUT 2
#define ERR 3
#define NO 4


int get_command(char*, size_t);
int execute(char*);
char* strstrip(char*);
int get_out_fds(char*, int*, int*);
char** parse_options(char*, int*, int*);
