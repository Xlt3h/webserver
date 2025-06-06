#include "httpparser.h"
void parse_the_http(char *request, int sfd)
{
    char logmsg[128];
    snprintf(logmsg, sizeof(logmsg), "Responding to request on fd %d", sfd);
    write_log(logmsg, NORMAL);

    const char *simple_html = "<html> \
    <head> \
    </head> \
    <body> \
    <h1> welcome to programming </h1> \
    </body> \
    </html>";

    char first_line[256];
    char method[16], path[128], http_version[16];

    int get_first_line = sscanf(request, "%[^\r\n]", first_line);

    if (get_first_line == 1)
    {
        if (sscanf(first_line, "%15s %127s %15s", method, path, http_version) == 3)
        {
            size_t body_len = strlen(simple_html);
            char content_length[64];
            snprintf(content_length, sizeof(content_length), "Content-Length: %zu\r\n", body_len);

            char response_headers[512];
            snprintf(response_headers, sizeof(response_headers),
                     "HTTP/1.1 200 OK\r\n"
                     "Date: Wed, 06 Jun 2025 10:00:00 GMT\r\n"
                     "Server: Apache/2.4.41 (Unix)\r\n"
                     "Content-Type: text/html; charset=UTF-8\r\n"
                     "%s"
                     "Connection: keep-alive\r\n"
                     "\r\n",
                     content_length);

            char full_response[2048];
            int written = snprintf(full_response, sizeof(full_response), "%s%s", response_headers, simple_html);

            send(sfd, full_response, written, 0);
        }
        else
        {
            write_log("invalid request!!!", WARNING);
        }
    }
    else
    {
        write_log("first line of the request is invalid", WARNING);
    }
}
