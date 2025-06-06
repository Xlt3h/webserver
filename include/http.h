#ifndef HTTP_H
#define HTTP_H
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef struct {
    int port;
    struct sockaddr;
}server_http_conf;
int start_server;
#endif