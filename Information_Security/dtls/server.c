#include "helper.h"
#include <openssl/bio.h>
#include <openssl/types.h>
#include <poll.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <sys/socket.h>

int main() {
  SSL_CTX *ctx = NULL;
  int sockfd = -1;
  struct sockaddr_in src, tar;
  SSL *ssl = NULL;
  char buf[256];
  BIO *bio = NULL;
  struct pollfd fds = {0};
  struct timeval timeout;

  openssl_init();

  if((ctx = SSL_CTX_new(DTLS_server_method())) == NULL)
     goto error;

  if(openssl_load_cert_files(ctx)) {
    perror("load_cert_files");
    goto error;
  }


  if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket");
  }

  if(init_sockaddr((struct sockaddr *)&src, AF_INET, sockfd, SERVER)) {
    perror("init_sockaddr");
    goto error;
  }

  if((ssl = SSL_new(ctx)) == NULL)
    goto error;

  if((bio = BIO_new_dgram(sockfd, BIO_NOCLOSE)) == NULL)
     goto error;

  timeout.tv_sec = 0;
  timeout.tv_usec = 25000;
  BIO_ctrl(bio, BIO_CTRL_DGRAM_GET_RECV_TIMEOUT, 0, &timeout);
  BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_SEND_TIMEOUT, 0, &timeout);

  SSL_set_bio(ssl, bio, bio);
  SSL_set_accept_state(ssl);

  while(1) {
    memset(&fds, 0, sizeof(fds));
    fds.fd = sockfd;
    fds.events = POLLIN;
    int ret = poll(&fds, 1, 10000);

    if(ret && (fds.revents & POLLIN)) {
      SSL_read(ssl, buf, 255);
      SSL_write(ssl, buf, 256);
    }

  }

  error:
  if(ctx)
    SSL_CTX_free(ctx);
  if(sockfd > 0)
    close(sockfd);
  return 0;
}
