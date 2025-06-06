
#include <stdio.h>
#include "log.h"
#include "serverconf.h"
#include "masternginx.h"
int main()
{
    write_log("Reading Server configuration !!!" , NORMAL);
    config_load = read_conf(SERVER_CONF_FILE_MICRO);
    //conf *config_load = read_conf(SERVER_CONF_FILE_MICRO);
    create_workers();
    while(1){}
    return 0;
}