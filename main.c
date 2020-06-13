
#include <stdio.h>
#include <string.h>
#include "log.h"

int main(int argc,char *argv[])
{
    #ifdef WIN32
    char *logdir="c:\\testlog";
    #else
    char *logdir="/tmp/testlog";
    #endif

    char *appname = "mytest";

    log_init(logdir,appname);

    log_info("info test.idx=%d,msg=%s.", 123, "hello");
    log_warn("warn test.idx=%d,msg=%s.", 123, "hello");
    log_debug("debug");


    log_destory();
}

