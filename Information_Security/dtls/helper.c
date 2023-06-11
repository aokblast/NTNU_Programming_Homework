#include "helper.h"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <sys/socket.h>

void
openssl_init() {
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
}

int
openssl_load_cert_files(SSL_CTX *ctx) {
  if(SSL_CTX_use_certificate_chain_file(ctx, "cert.pem") == -1)
    return -1;
  if(SSL_CTX_use_RSAPrivateKey_file(ctx, "private.key", SSL_FILETYPE_PEM) == 1)
    return -1;
  return 0;
}


int
init_sockaddr(struct sockaddr *sockaddr, int sock, int sockfd, enum SC_stat stat) {
  struct sockaddr_in addr;
  int on  = 1;
  memset(&addr, 0, sizeof(addr));

  addr.sin_family = sock;

  switch(stat) {
  case SERVER:
    addr.sin_port = htons(5555);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
      return -1;
    if(bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))
      return -1;
    break;
  case CLIENT:
    addr.sin_port = htons(5000);
    inet_aton("127.0.0.1", (struct in_addr *)&addr.sin_addr.s_addr);
    break;
  }

  if(sockaddr)
    memcpy(sockaddr, &addr, sizeof(addr));

  return 0;
}
