#include "masternginx.h"

int create_workers()
{
    int number_of_workers = config_load->work_processes;

    write_log("creating workers", NORMAL);
    int sockfd = worker_init();
    for (int i = 0; i < number_of_workers; i++)
    {
        const char *msg = "creating worker number ";
        char full_msg[64];
        snprintf(full_msg, sizeof(full_msg), "%s %d", msg, i);

        write_log(full_msg, NORMAL);
        int create_process = fork();

        if (create_process == -1)
        {
            write_log("cannot create a worker", ERROR);
            return EXIT_FAILURE;
        }
        else if (create_process == 0)
        {
            worker_handle(sockfd);
        }
        else
        {
            write_log("main process is working", NORMAL);
        }
    }

}