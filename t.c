#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_EVENTS 10
#define PORT 8080

// Helper: set fd non-blocking
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    int listen_fd, epfd;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    addr.sin_port = htons(PORT);

    // 1. Create listening socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set reuse addr
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    if (listen(listen_fd, SOMAXCONN) == -1) {
        perror("listen");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    if (set_nonblocking(listen_fd) == -1) {
        perror("set_nonblocking listen_fd");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    // 2. Create epoll instance
    epfd = epoll_create1(0);
    if (epfd == -1) {
        perror("epoll_create1");
        close(listen_fd);
        return EXIT_FAILURE;
    }

    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];

    // 3. Add listen_fd to epoll, watch for EPOLLIN (incoming connections)
    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &event) == -1) {
        perror("epoll_ctl: listen_fd");
        close(listen_fd);
        close(epfd);
        return EXIT_FAILURE;
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (n == -1) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == listen_fd) {
                // 4. New incoming connection(s)
                while (1) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
                    if (client_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // No more connections to accept
                            break;
                        } else {
                            perror("accept");
                            break;
                        }
                    }

                    // Make client socket non-blocking
                    if (set_nonblocking(client_fd) == -1) {
                        perror("set_nonblocking client_fd");
                        close(client_fd);
                        continue;
                    }

                    // Add client_fd to epoll monitoring for EPOLLIN (data to read)
                    event.events = EPOLLIN | EPOLLET; // Edge-triggered for demo
                    event.data.fd = client_fd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                        perror("epoll_ctl: client_fd");
                        close(client_fd);
                        continue;
                    }

                    char client_ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                    printf("Accepted connection from %s:%d, fd=%d\n", client_ip, ntohs(client_addr.sin_port), client_fd);
                }
            } else {
                // 5. Data from client socket
                int client_fd = events[i].data.fd;
                if (events[i].events & EPOLLIN) {
                    char buf[512];
                    ssize_t count = read(client_fd, buf, sizeof(buf));
                    if (count == -1) {
                        if (errno != EAGAIN) {
                            perror("read");
                            close(client_fd);
                        }
                    } else if (count == 0) {
                        // Client closed connection
                        printf("Client fd %d disconnected\n", client_fd);
                        close(client_fd);
                    } else {
                        // Echo back what was read
                        write(client_fd, buf, count);
                    }
                }
            }
        }
    }

    close(listen_fd);
    close(epfd);
    return 0;
}
