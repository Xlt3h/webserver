
#include <stdio.h>
#include "http.h"
#include "log.h"
#include "serverconf.h"
int main()
{
    write_log("Reading Server configuration !!!" , NORMAL);
    config_load = read_conf(SERVER_CONF_FILE_MICRO);
    //conf *config_load = read_conf(SERVER_CONF_FILE_MICRO);

    return 0;
}