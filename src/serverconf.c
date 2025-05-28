
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "log.h"
#include "serverconf.h"


conf* read_conf(const char* conf_file){
    conf* server_conf = (conf*)malloc(sizeof(conf));
    FILE* server_conf_file = fopen(SERVER_CONF_FILE_MICRO , "r");
    fseek(server_conf_file,0,SEEK_END);
    long server_conf_file_bytes = ftell(server_conf_file);
    rewind(server_conf_file);

    int file_size_byte  = server_conf_file_bytes +1;
    char* file_buffer  = malloc(file_size_byte);
    if(file_buffer == NULL){
        write_log("Cannot allocate memory for file content!!!" , ERROR);
        fclose(server_conf_file);
        //return EXIT_FAILURE;
    }
    int line_bytes = 100;
    char  line[line_bytes], key[30] , value[75];

    // TODO: DON'T FORGET TO IMPLEMENT strtol CAUSE ITS THE ONE FOR ERROR HANDLING WHEN CONVERTING STRING TO INT
    while(fgets(line, line_bytes, server_conf_file)){
        if(sscanf(line, "%63[^=]=%63s" , key,value) == 2){
            
            if(strcmp(key,"port") ==0){
                server_conf->port = atoi(value);
                printf("%d" ,server_conf->port);
            }
            else if(strcmp(key,"work_processes") ==0){
                server_conf->work_processes = atol(value);

            }
            else if(strcmp(key,"max_connections") == 0 ){
                server_conf->max_connections = atol(value);

            }
            else if(strcmp(key,"keep_alive_timeout") == 0){
                server_conf->keep_alive_timeout = atol(value);
                
            }
            else{
                int illegal_msg_size = 75;
                char illegal_msg[illegal_msg_size];
                snprintf(illegal_msg,illegal_msg_size,"Illegal server configuration: %s", line);
                write_log(illegal_msg,WARNING);
            }
        }
    }
    

    fclose(server_conf_file);
    free(file_buffer);
    return server_conf;
}