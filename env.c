#include<stdio.h>
#include<stdlib.h>

int
main(void){
  char *pass;
  
  pass = getenv("PASS");
  printf("the pass is %s\n", pass);

  return 0;
}
