/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_run.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
//TF处理
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
//时间
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <cstring>
#include <iostream>
#include <sstream>

static int isRunning(const char *procname)
{
    int ret = 0;
    char sCurrPid[16] = {0};
    sprintf(sCurrPid, "%d\n", getpid());

    FILE *fstream=NULL;
    char buff[1024] = {0};

    snprintf(buff,1024,"ps | grep %s | grep -v grep | awk '{print $1}'",procname);
    if(NULL==(fstream=popen(buff, "r")))
    {
        fprintf(stderr,"execute command failed: %s", strerror(errno));
        return -1;
    }
    memset(buff, 0, sizeof(buff));
    while(NULL!=fgets(buff, sizeof(buff), fstream)) {
        if (strlen(buff) > 0) {
            if (strcmp(buff, sCurrPid) !=0) {
                //printf("******%s, %s****\n", buff, sCurrPid);
                ret = 1;
                break;
            }
        }
    }
    pclose(fstream);
    return ret;
}

bool App_Run::isRun(const char *procname)
{
    int ret = isRunning(procname);
    return ret == 1 ? true:false;
}
