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
    if(QFile::exists("/sys/clever/beep/switch")) {
        QTimer::singleShot(1,this,SLOT(beep_initFunSlot()));
    } else cout << "Buzzer driver error";
}

App_Buzzer::~App_Buzzer()
{
    int *beep = mBeep;
    close(beep[0]);
    close(beep[1]);
}

void App_Buzzer::beep_initFunSlot()
{
    int *beep = mBeep;
    beep[0] = open("/sys/clever/beep/switch", O_RDWR);
    if(beep[0] < 0) perror("open beep switch"); else BEEP_ON(beep[0]);

    beep[1] = open("/sys/clever/beep/trigger", O_RDWR);
    if(beep[1] < 0) perror("open beep trigger");
    QTimer::singleShot(5*1000,this,SLOT(beep_offSlot()));

    mBeepTimer = new QTimer(this); mBeepTimer->start(1000);
    connect(mBeepTimer, SIGNAL(timeout()), this, SLOT(beep_onTimeoutDone()));
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
    if(dev->alarm && dev->cfg.param.buzzerSw) {
        if(!mBeepRun) {
            BEEP_TRIGGER(mBeep[1], "500");
            mBeepRun = true;
        }
    } else if(mBeepRun) beep_offSlot();
}

void App_Buzzer::beep_onTimeoutDone()
{
    beep_workDown();
}
