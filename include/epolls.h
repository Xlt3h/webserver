#ifndef EPOLLS_H
#define EPOLLS_H
#include <sys/epoll.h>
#include "log.c"
#include <fcntl.h>
int set_non_block(int fd);
int create_epoll();
int add_epoll(int epfd, int fd, struct epoll_event event);
#endif