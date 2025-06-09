#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

int write_log(char *message, type_log type_of_log)
{
    // FILE *log_file = fopen(LOG_FILE, "a");
    // if (log_file == NULL)
    // {
    //     perror("failed to open the log file, creating one!");
    //     return EXIT_FAILURE;
    // }
    // // define the time stamp
    // time_t now;
    // time(&now);
    // struct tm *local_time = localtime(&now);
    // char time_str[50];
    // strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S ", local_time);

    // const char *type;
    // switch (type_of_log)
    // {
    // case ERROR:
    //     type = " [ERROR] : ";
    //     break;
    // case NORMAL:
    //     type = " [NORMAL] : ";
    //     break;
    // case WARNING:
    //     type = " [WARNING] : ";
    //     break;
    // default:
    //     type = " [UNKNOWN] : ";
    //     break;
    // }

    // int msg_length = strlen(time_str) + strlen(type) + strlen(message) + 1;
    // char *log_message = malloc(msg_length);

    // if (!log_message)
    // {
    //     perror("memory allocation failed");
    //     fclose(log_file);
    //     return EXIT_FAILURE;
    // }

    // snprintf(log_message, msg_length, "%s%s%s", time_str, type, message);

    // if (fprintf(log_file, "%s\n", log_message) < 0)
    // {
    //     perror("can't write to the file!!!");
    //     free(log_message);
    //     fclose(log_file);
    //     return EXIT_FAILURE;
    // }

    // free(log_message);
    // fclose(log_file);
    return EXIT_SUCCESS;
}