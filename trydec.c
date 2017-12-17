#include"crysh.h"
#include<openssl/evp.h>
#include<openssl/aes.h>
#include<fcntl.h>

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
  
  buff = NULL;
  nr = getline(&buff, &nr, stdin);
  if((strncmp(buff, "Salted__",8))!=0){
    fprintf(stderr, "unable to decrypt exiting\n");
    return(128);
  }
  memcpy(salt, buff+PKCS5_SALT_LEN, PKCS5_SALT_LEN);

  EVP_BytesToKey(cipher, digest, salt, (unsigned char*)pass, passlen, 1, key, iv);

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
  // printf("decrypted data: %s\n", c);
  get_command(c, strlen(c));
  
  free(commands);
  free(buff);
  
 
  
  return 0;
}
