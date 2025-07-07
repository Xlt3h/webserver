#include "httpparser.h"
#include <sys/stat.h>

void parse_the_http(char *request, int sfd)
{
    char logmsg[128];
    snprintf(logmsg, sizeof(logmsg), "Responding to request on fd %d", sfd);
    write_log(logmsg, NORMAL);

    const char *file_path = "./static/bigpage.html";

    char first_line[256], method[16], path[128], http_version[16];
    if (sscanf(request, "%[^\r\n]", first_line) == 1 &&
        sscanf(first_line, "%15s %127s %15s", method, path, http_version) == 3)
    {
        if (strcmp(path, "/") == 0) {
            int file_fd = open(file_path, O_RDONLY);
            if (file_fd < 0) return;

            struct stat file_stat;
            if (fstat(file_fd, &file_stat) < 0) {
                close(file_fd);
                return;
            }

            char headers[512];
            int header_len = snprintf(headers, sizeof(headers),
                "HTTP/1.1 200 OK\r\n"
                "Date: Wed, 06 Jun 2025 10:00:00 GMT\r\n"
                "Server: Apache/2.4.41 (Unix)\r\n"
                "Content-Type: text/html; charset=UTF-8\r\n"
                "Content-Length: %ld\r\n"
                "Connection: keep-alive\r\n\r\n", file_stat.st_size);

            send(sfd, headers, header_len, 0);
            sendfile(sfd, file_fd, NULL, file_stat.st_size);
            close(file_fd);
            return;
        } else {
            const char* not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
            send(sfd, not_found, strlen(not_found), 0);
            return;
        }
    } else {
        write_log("invalid request!!!", WARNING);
    }
}
