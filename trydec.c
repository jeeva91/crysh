#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<openssl/evp.h>
#include<openssl/aes.h>
#include"crysh.h"


#define BUFFSIZE 256
int
main(void){
  size_t nr;
  char* buff;
  char* pass;
  unsigned char salt[PKCS5_SALT_LEN];
  unsigned char key[32];
  unsigned char  iv[16];
  EVP_CIPHER_CTX d_ctx;
  size_t passlen;
  const EVP_CIPHER *cipher;
  const EVP_MD *digest;

  if((pass = (char*)getenv("CRYSH_PASSWORD"))==NULL){
    if((pass = getpass("Enter the password for the CRYsh :" ))==NULL){
      fprintf(stderr, "no password");
      exit(128);
    }
  }
  passlen = (unsigned int) strlen(pass);
  //  printf("the password is %s\n", pass);

  cipher = EVP_aes_256_cbc();

  if(cipher == NULL){
    perror("cipher failed");
    exit(128);
  }
  digest = EVP_sha1();
  if(digest == NULL){
    perror("digest failed");
    exit(128);
  }
  /*
  //malloc will be done by getline

  if((buff = malloc(BUFFSIZE))==NULL){
    perror("malloc failed");
    exit(128);
    }

  //To get the input from the file

  int input;
  if((input = open("encdata", O_RDONLY))<0){
    perror("unalbe to open the file encdata");
  }
  
  if((nr = read(input, buff, BUFFSIZE))==0){
    perror("unable to read the file");
    }
  close(input);
  
  */

  nr = getline(&buff, &nr, stdin);
  
  memcpy(salt, buff+PKCS5_SALT_LEN, PKCS5_SALT_LEN);

  EVP_BytesToKey(cipher, digest, salt, (unsigned char*)pass, passlen, 1, key, iv);

  /*  size_t i;  

  printf("salt = ");
  for(i=0;i<8;i++){
  printf("%02X", salt[i]);
  }

  printf("\nkey = ");
  for(i=0;i<32;i++){
    printf("%02X", key[i]);
  }

  printf("\niv = ");
  for(i=0;i<16;i++){
    printf("%02X", iv[i]);
  }
  printf("\n");



  printf("encrypted data is ");
  for(i=0;i<nr-16;i++){
    
    printf("%02x",buff[16+i]);
  }
  printf("\n");
  */
  //EVP init  
  EVP_CIPHER_CTX_init(&d_ctx);
  
  EVP_DecryptInit_ex(&d_ctx, cipher, NULL, key, iv);

  char* commands;
  commands = malloc(nr);
  int lencommands=nr;
  int lenbuff = nr-16;
  EVP_DecryptUpdate(&d_ctx, (unsigned char*)commands, &lencommands, (unsigned char*)buff+16, lenbuff);
  // EVP_DecryptFinal(&d_ctx, (unsigned char*)commands, &lencommands);
  char * c;
  c = strsep(&commands, "\n");
  printf("decrypted data: %s\n", c);
  get_command(c, strlen(c));
  
  free(commands);
  free(buff);
  
 
  
  return 0;
}
