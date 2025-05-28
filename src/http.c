
#include "log.h"
#include "serverconf.h"
#include <string.h>
#include <netinet/in.h>
#include "unistd.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int start_server(){

    //log file 
    
    conf* configuration  = read_conf(SERVER_CONF_FILE_MICRO);
    printf("%d", configuration->port);
    struct  sockaddr_in serveraddr , clientaddr;
    socklen_t clientaddr_size = sizeof(clientaddr);
    
    serveraddr.sin_family  = AF_INET;
    serveraddr.sin_port = htons(configuration->port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;


    int socket_server, client_server;

    write_log("Starting the server!!!" , NORMAL);
    socket_server = socket(AF_INET,SOCK_STREAM,0);

    if(socket_server < 0){
    
        write_log("Can't start the socket", ERROR);
        write_log("Exiting the system" , ERROR);
        return EXIT_FAILURE;
    }
    if(bind(socket_server,(struct sockaddr*)&serveraddr , sizeof(serveraddr)) < 0){
          write_log("Can't bind the address exiting", ERROR);
          return EXIT_FAILURE;
    }
    if(listen(socket_server ,5) < 0){
         write_log("Can't listen to the incoming ", ERROR);
        return EXIT_FAILURE;
    }
    while(1)
    {
        int client = accept(socket_server,(struct sockaddr*)&clientaddr ,(socklen_t *) &clientaddr_size);
        char* data = "this is the nginx lighter!!!\n";
        
        send(client,data,strlen(data),0);
        close(client);
    }
    close(socket_server);
    return 0;
}