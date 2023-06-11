#include <asm-generic/ioctls.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/types.h>
#include <openssl/bio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include "helper.h"

int main() {
  SSL_CTX* ctx = NULL;
  SSL *ssl;
  BIO *bio;
  int sockfd = -1;

  openssl_init();
  if((ctx = SSL_CTX_new(DTLS_client_method())) == NULL)
    goto error;

  if(openssl_load_cert_files(ctx))
    goto error;

  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    goto error;

  struct sockaddr_in dst;
  struct sockaddr peer;

  if(init_sockaddr((struct sockaddr*)&dst, AF_INET, sockfd, CLIENT))
    goto error;

  if((ssl = SSL_new(ctx)) == NULL)
    goto error;

  if((bio = BIO_new_dgram(sockfd, BIO_NOCLOSE)) == NULL)
    goto error;

  BIO_ctrl_set_connected(bio, &peer);
  SSL_set_bio(ssl, bio, bio);
  SSL_set_connect_state(ssl);

  char buf[256];

  while(1) {
    if(feof(stdin))
      break;
    fgets(buf, 256, stdin);
    SSL_write(ssl, buf, strlen(buf));
    SSL_read(ssl, buf, 256);
    printf("%s\n", buf);
  }


  error:
     if(ctx)
       SSL_CTX_free(ctx);
     if(sockfd != -1)
       close(sockfd);

     return 0;
}
