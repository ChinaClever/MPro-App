/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_sensor.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

App_Sensor::App_Sensor(QObject *parent)
    : App_Led{parent}
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    if(QFile::exists("/sys/clever/tem_hum/th0/data")) {
        QtConcurrent::run(this, &App_Sensor::env_run);
    } else cout << "env err";
#endif
}

App_Sensor::~App_Sensor()
{
    mEnvIsRun = false;
    env_close();
}

void App_Sensor::env_close()
{
    int *th = mFds;
    for(int i=0; i<2; i++) {
        if(th[i]>=0) close(th[i]);
    }
}

void App_Sensor::env_initFun()
{
    QString fmd = "/sys/clever/tem_hum/th%1/data";
    int *th = mFds; for(int i=0; i<2; ++i) {
        QString cmd = fmd.arg(i);
        th[i] = open(cmd.toLocal8Bit().data(), O_RDONLY);
        if(th[i] < 0) cout << "env open err" << i << fmd;
        //else mEnvIsRun = true;
    }
}

int App_Sensor::door_initFun()
{
    int fd = open("/dev/door", O_RDONLY);
    if(fd < 0) cout << "open /dev/door failed";
    return fd;
}

void App_Sensor::door_workDown()
{
    static int fd = door_initFun();
    int data[2]; if(fd < 0) return;
    sEnvData *env = &cm::masterDev()->env;
    int ret = read(fd, data, sizeof(data));
    if(ret < 0) cout << "read /dev/door failed";
    else for(int i=0; i<2; ++i)  env->door[i] = data[i] +1;
    // close(fd); cout << data[0] +1 << data[1] +1;
}

void App_Sensor::env_workDown()
{
    sEnvData *env = &cm::masterDev()->env;
    int *th = mFds; char t[32]; uint v[2];
    for(int i=0; i<2; ++i) {
        if(th[i] >= 0) {
            if(read(th[i], t, sizeof(t)) < 0) {
                env->isInsert[i] = 0;
            } else {
                sscanf(t, "%d:%d", v, v+1);
                if((v[0] < 100) && (v[1] < 100)) {
                    env->isInsert[i] = 1;
                    env->hum.value[i] = v[1];
                    env->tem.value[i] = v[0]*10;
                }
            }
        } else env->isInsert[i] = 0;
    }
}

void App_Sensor::env_delay()
{
    int r = QRandomGenerator::global()->bounded(565);
    if(cm::runTime() > 48*60*60){r += 1000;}  cm::mdelay(3500 + r);
}

void App_Sensor::env_run()
{
    cm::mdelay(2111);
    while(mEnvIsRun) {
        door_workDown();
        env_initFun();
        env_workDown();
        env_close();
        env_delay();
    }
}
