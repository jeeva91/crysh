#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<openssl/evp.h>
#include<openssl/aes.h>



#define BUFFSIZE 256
int
main(void){
  int input, nr;
  char* buff;
  char* pass;
  char salt[9];
  unsigned char key[32];
  unsigned char  iv[32];
  const EVP_CIPHER *cipher;
  const EVP_MD *digest = NULL;
  pass = getenv("PASS");
  cipher = EVP_get_cipherbyname("aes-256-cbc");

  if(cipher == NULL)
    ErrorMessage("cipher failed");

  digest = EVP_get_digestbyname("SHA1");
  if(digest == NULL)
    ErrorMessage("digest failed");
  
  if((buff = malloc(BUFFSIZE))==NULL){
    perror("malloc failed");
  }
  if((input = open("encdata", O_RDONLY))<0){
    perror("unalbe to open the file encdata");
  }

  if((nr = read(input, buff, BUFFSIZE))<0){
    perror("unable to read the file");
  }
  printf("read the following %s\n%d\n", buff, nr);
  strncpy(salt, buff+8, 8);
  printf("the salt is %s\n", salt);
  EVP_BytesToKey(cipher, digest, salt, (unsigned char*)pass, strlen(pass), 1, key, iv)
  
  return 0;
}
