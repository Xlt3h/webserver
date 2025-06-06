#ifndef WORKER_H
#define WORKER_H
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/net.h>
#include <unistd.h>
#include "serverconf.h"
#include "log.h"
#include <stdlib.h>
#include "epolls.h"
#include <errno.h>
#include "httpparser.h"
int worker_init();
int worker_handle(int sockfd);

#endif