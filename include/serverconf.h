#ifndef LOGSTRUCT_H
#define LOGSTRUCT_H
#define SERVER_CONF_FILE_MICRO "/home/rich/Documents/cplus/webserver/server.conf"
typedef struct{
    int port;
    long work_processes;
    long max_connections;
    long  keep_alive_timeout;
}conf;
conf* read_conf(const char* conf_file);
#endif