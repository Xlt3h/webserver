#ifndef HTTPPARSER_H
#define HTTPPARSER_H
#include "log.h"
#include <string.h>
#include  <stdio.h>
#include <stdio.h>
#include <netinet/in.h>

void parse_the_http(const char *request, int sfd);
#endif