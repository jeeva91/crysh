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
  unsigned char* buff;
  unsigned char* pass;
  unsigned char salt[PKCS5_SALT_LEN];
  unsigned char key[32];
  unsigned char  iv[16];
  const EVP_CIPHER *cipher;
  const EVP_MD *digest;
  pass = (unsigned char*)getenv("PASS");
  printf("the password is %s\n", pass);
  cipher = EVP_aes_256_cbc();

  if(cipher == NULL)
    perror("cipher failed");

  digest = EVP_sha1();
  if(digest == NULL)
    perror("digest failed");
  
  if((buff = malloc(BUFFSIZE))==NULL){
    perror("malloc failed");
  }
  if((input = open("encdata", O_RDONLY))<0){
    perror("unalbe to open the file encdata");
  }

  if((nr = read(input, buff, BUFFSIZE))<0){
    perror("unable to read the file");
  }
  close(input);
  memcpy(salt, buff+8, PKCS5_SALT_LEN);

  EVP_BytesToKey(cipher, digest, salt, pass, (int)strlen(pass), 1, key, iv);

  //  printf("key = %s\niv = %s\n", key,iv);
  // printf("the salt length is %d\n", PKCS5_SALT_LEN);
  
  printf("salt = ");
  printf("%02X", salt[0]);
  printf("%02X", salt[1]);
  printf("%02X", salt[2]);
  printf("%02X", salt[3]);
  printf("%02X", salt[4]);
  printf("%02X", salt[5]);
  printf("%02X", salt[6]);
  printf("%02X", salt[7]);

  printf("\nkey = ");
  int i;
  for(i=0;i<32;i++){
    printf("%02X", key[i]);
  }

  printf("\niv = ");
  
  for(i=0;i<16;i++){
    printf("%02X", iv[i]);
  }

  printf("\n");
  return 0;
}
