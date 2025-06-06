#include "httpparser.h"

void parse_the_http(const char *request, int sfd)
{
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

    char *respone = "HTTP/1.1 200 OK\r\n"
                    "Date: Wed, 06 Jun 2025 10:00:00 GMT\r\n"
                    "Server: Apache/2.4.41 (Unix)\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "Content-Length: 1234\r\n"
                    "Connection: keep-alive\r\n"
                    "\r\n";

    char full_response[2048];

    if (get_first_line == 1)
    {
        if (sscanf(first_line, "%15s %128s %15s", method, path, http_version) == 3)
        {
            
            snprintf(full_response, sizeof(full_response), "%s%s", respone, simple_html);
            send(sfd,full_response, sizeof(full_response),0);
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
