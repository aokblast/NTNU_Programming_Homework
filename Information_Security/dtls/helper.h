#pragma once

#include <openssl/ssl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

enum SC_stat {
  SERVER,
  CLIENT
};

void openssl_init();
int openssl_load_cert_files(SSL_CTX* ctx);
int init_sockaddr(struct sockaddr *sockaddr, int sock, int sockfd, enum SC_stat stat);

