#include "epolls.h"

int set_non_block(int fd)
{
    int get_flags = fcntl(fd, F_GETFL, 0);
    if (fcntl(fd, F_SETFL, get_flags | O_NONBLOCK) < 0)
    {
        write_log("cannot set the file descriptor to non block", ERROR);
        return -1;
        //return EXIT_FAILURE;
    }
    return 0;
}
int create_epoll()
{
    int epfd = epoll_create1(0);
    if (epfd < 0)
    {
        write_log("cannot start the epoll", ERROR);
        return -1;
        //return EXIT_FAILURE;
    }
    return epfd;
}
int add_epoll(int epfd, int fd, struct epoll_event event)
{
    int epfdctl = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, (struct epoll_event *)&event);
    if (epfdctl < 0)
    {
        write_log("cannot add the event to the epoll", ERROR);
        return -1;
        //return EXIT_FAILURE;
    }
    return epfdctl;
}