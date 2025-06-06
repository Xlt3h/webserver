#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#define MAX_POOL 10
#define PORT 8036
int main()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // define the server addr_in structure
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_family = AF_INET;

    int sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd < 0)
    {
        perror("can't start the socket!!!");
        return EXIT_FAILURE;
    }
    int socket_flag = fcntl(sfd, F_GETFL, 0);
    fcntl(sfd, F_SETFL, socket_flag | O_NONBLOCK);

    if (bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("cannot bind the address");
        return EXIT_FAILURE;
    }
    if (listen(sfd, 5) < 0)
    {
        perror("cannot listen to the incoming connection !!!");
        return EXIT_FAILURE;
    }

    // start the epoll

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("cannot start the epoll !!!");
        return EXIT_FAILURE;
    }
    struct epoll_event event;
    struct epoll_event events[MAX_POOL];

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET; // IN is for anything coming in and let is to let me once i dont have to wake up everytime

    int socket_epoll_control = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sfd, &event);
    if (socket_epoll_control == -1)
    {
        perror("cannot add epoll !!!");
        return EXIT_FAILURE;
    }
    while (1)
    {
        int wait_for_epoll_event = epoll_wait(epoll_fd, (struct epoll_event *)&events, MAX_POOL, -1);
        if (wait_for_epoll_event == -1)
        {
            perror("cannot wait for epoll events !!!");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < wait_for_epoll_event; i++)
        {
            if (events[i].data.fd == sfd)
            {
                while (1)
                {
                    int accept_client = accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len);
                    if (accept_client == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            close(events[i].data.fd);
                            break;
                        }

                        else
                        {
                            perror("cannot accept the incoming connections!!!");
                            return EXIT_FAILURE;
                        }
                    }
                    int get_accept_client_flags = fcntl(accept_client, F_GETFL, 0);
                    int set_accept_client_flags = fcntl(accept_client, F_SETFL,get_accept_client_flags | O_NONBLOCK);
                    if (set_accept_client_flags < 0)
                    {
                        perror("cannot set client to non blocking !!!");
                        return EXIT_FAILURE;
                    }
                    event.events = EPOLLIN | EPOLLET;
                    event.data.fd = accept_client;
                    int add_accept_client_epoll = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_client, &event);
                    if (add_accept_client_epoll < 0)
                    {
                        perror("i cannot add accept client to the epoll!!!");
                        return EXIT_FAILURE;
                    }
                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                    printf("accepted a connection from %s:%d \n sending a welcoming message\n", client_ip, ntohs(client_addr.sin_port));
                    char msg[128];
                    strcpy(msg, "hey welcome we are using a epoll for this server faster boy!!!");
                    send(accept_client, msg, 128, 0);
                }
            }
            else
            {
                char recv_msg[256];
                ssize_t count = recv(events[i].data.fd, recv_msg, 256, 0);
                if (count == -1)
                {
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        close(events[i].data.fd);
                    }
                }
                else if (count == 0)
                {
                    printf("Client disconnected\n");
                    close(events[i].data.fd);
                }
                else
                {
                    //memset(recv_msg,0,sizeof(recv_msg));
                    printf("recieved message:\n %s\n",recv_msg);
                    char send_msg[256];
                    printf("enter your message to send to the client:>");
                    fgets(send_msg,sizeof(send_msg),stdin);
                    send(events[i].data.fd, send_msg, 256, 0);
                }
            }
        }
    }
    close(sfd);
    close(epoll_fd);
    return 0;
}
