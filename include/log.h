#ifndef LOG_H
#define LOG_H

typedef enum{
    WARNING,
    NORMAL,
    ERROR
} type_log;

#define LOG_FILE "/home/rich/Documents/cplus/webserver/watch.log"

int write_log(char* message , type_log type_of_log);
#endif