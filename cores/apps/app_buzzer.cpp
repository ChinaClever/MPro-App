/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_buzzer.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BEEP_ON(fd) write(fd,"on",2)
#define BEEP_OFF(fd) write(fd,"off",3)
#define BEEP_TRIGGER(fd, ms) write(fd, ms, strlen(ms))

App_Buzzer::App_Buzzer(QObject *parent)
    : QObject{parent}
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    if(QFile::exists("/sys/clever/beep/switch")) {
        QTimer::singleShot(1,this,&App_Buzzer::beep_initFunSlot);
    } else cout << "beep error";
#endif
}

App_Buzzer::~App_Buzzer()
{
    int *beep = mBeep;
    close(beep[0]);
    close(beep[1]);
}

void App_Buzzer::beep_once()
{
    int *beep = mBeep; BEEP_ON(beep[0]);
    QTimer::singleShot(213,this,&App_Buzzer::beep_offSlot);
}

void App_Buzzer::beep_initFunSlot()
{
    int *beep = mBeep;
    beep[0] = open("/sys/clever/beep/switch", O_RDWR);
    if(beep[0] < 0) perror("open beep switch"); else beep_once();

    beep[1] = open("/sys/clever/beep/trigger", O_RDWR);
    if(beep[1] < 0) perror("open beep trigger");

    mBeepTimer = new QTimer(this); mBeepTimer->start(1000);
    connect(mBeepTimer, &QTimer::timeout, this, &App_Buzzer::beep_onTimeoutDone);
}

void App_Buzzer::beep_offSlot()
{
    int *beep = mBeep;
    BEEP_OFF(beep[0]); cm::mdelay(1);
    BEEP_TRIGGER(beep[1], "0");
    mBeepRun = false;
}

void App_Buzzer::beep_workDown()
{
    sDevData *dev = cm::masterDev();
    int sw = dev->cfg.param.buzzerSw;
    if(dev->alarm && (sw==1)) {
        if(!mBeepRun) {
            BEEP_TRIGGER(mBeep[1], "300");
            mBeepRun = true;
        }
    } else if(mBeepRun) beep_offSlot();
}

void App_Buzzer::beep_onTimeoutDone()
{
    beep_workDown();
}
