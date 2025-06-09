#ifndef HTTPPARSER_H
#define HTTPPARSER_H
#include "log.h"
#include <string.h>
#include  <stdio.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <fcntl.h>

#include <unistd.h>
void parse_the_http(char *request, int sfd);
#endif