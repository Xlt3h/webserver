#include "worker.h"
#include <string.h>
#define MAX_POOL 32
int worker_init()
{

    int opt = 1;
    write_log("creating socket !!!", NORMAL);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 1)
    {
        write_log("cannot start the socket", ERROR);
        return EXIT_FAILURE;
    }
    set_non_block(sock);
    int socket_opt = setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    if (socket_opt < 0)
    {
        write_log("cannot set the socket option !!!", ERROR);
        return EXIT_FAILURE;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(config_load->port);

    if (bind(sock, (struct sockaddr *)&server_addr, 0) < 0)
    {
        write_log("cannot bind the socket !!!", ERROR);
        return EXIT_FAILURE;
    }

    if (listen(sock, SOMAXCONN) < 0)
    {
        write_log("cannot listen to incoming connetion !!!", ERROR);
        return EXIT_FAILURE;
    }
    write_log("Socket is listening for incoming connection!!!", NORMAL);

    return sock;
}

int worker_handle()
{
    // define client
    struct sockaddr_in client_addr;
    socklen_t client_addr_socklen = sizeof(client_addr);

    int sock = worker_init();
    set_non_block(sock);
    int epfd = create_epoll();
    struct epoll_event event;
    struct epoll_event events[MAX_POOL];
    event.data.fd = sock;
    event.events = EPOLLIN | EPOLLET;
    int epoll_controller = add_epoll(epfd, sock, event);
    write_log("Adding socket to epoll is successfull !!!", NORMAL);
    int* accept_fd = NULL;
    while (1)
    {
        int number_of_event = epoll_wait(epfd, &events, MAX_POOL, -1);
        for (int i = 0; i < number_of_event; i++)
        {
            if (events[i].events == EPOLLIN) // new connection trying to connect
            {
                while (1)
                {
                    int accept_clients_fd = accept(sock, (struct sockaddr *)&client_addr, &client_addr_socklen);
                    accept_fd = &accept_clients_fd;
                    if (accept_clients_fd < 0)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            write_log("Accepted all connections", NORMAL);
                            break;
                        }
                        else
                        {
                            write_log("cannot accept incoming connections", ERROR);
                            return EXIT_FAILURE;
                        }
                    }
                    set_non_block(accept_clients_fd);
                    event.data.fd = accept_clients_fd;
                    add_epoll(epfd,accept_clients_fd,event);
                    char addr_str[INET_ADDRSTRLEN];
                    //memset(addr_str,0,128);
                    inet_ntop(AF_INET,&client_addr.sin_addr, addr_str,INET_ADDRSTRLEN);
                    char accept_ip_log[50];
                    snprintf("Accepted connection from %s",sizeof(accept_ip_log),accept_ip_log ,addr_str);
                    write_log(accept_ip_log,NORMAL);

                }
            }
            else // to handle clients that have already connected
            {
                const char* recv_request[2048];
                recv(accept_fd ,recv_request,sizeof(recv_request),0);
                parse_the_http(recv_request, accept_fd);
            }
        }
    }
}