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
    int *th = mFds; mEnvIsRun = false;
    for(int i=0; i<2; i++) {
        if(th[i]) close(th[i]);
    }
}

void App_Sensor::env_initFun()
{
    QString fmd = "/sys/clever/tem_hum/th%1/data";
    int *th = mFds; for(int i=0; i<2; ++i) {
        QString cmd = fmd.arg(i);
        th[i] = open(cmd.toLocal8Bit().data(), O_RDONLY);
        if(th[i] < 0) cout << "env open err" << i << fmd;
        else mEnvIsRun = true;
    }
}

void App_Sensor::env_workDown()
{
    sEnvData *env = &cm::masterDev()->env;
    int *th = mFds; char t[32]; int v[2];
    for(int i=0; i<2; ++i) {
        if(th[i] >= 0) {
            if(read(th[i], t, sizeof(t)) < 0) {
                env->isInsert[i] = 0;
                env->tem.value[i] = 0;
                env->hum.value[i] = 0;
            } else {
                env->isInsert[i] = 1;
                sscanf(t, "%d:%d", v, v+1);
                env->hum.value[i] = v[1];
                env->tem.value[i] = v[0]*10;
                //qDebug() << i <<  v[0] << v[1];
                //printf("th%d:温度(%d),湿度(%d)\n", i, v[0], v[1]);
            }
        }
    }
}

void App_Sensor::env_delay()
{
    int r = QRandomGenerator::global()->bounded(565);
    if(cm::runTime() > 48*60*60){r += 500;}  cm::mdelay(1500 + r);
}

void App_Sensor::env_run()
{
    cm::mdelay(2111);
    while(mEnvIsRun) {
        env_initFun();
        env_workDown();
        env_close();
        env_delay();
    }
}
