#ifndef WORKER_H
#define WORKER_H
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/net.h>
#include "serverconf.h"
#include "log.h"
#include <stdlib.h>
#include "epolls.h"
#include "http.h"
#include "httpparser.h"
int worker_init();
#endif