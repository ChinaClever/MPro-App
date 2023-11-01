/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_led.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

enum {
    RGB_RED = 0,
    RGB_GREEN,
    RGB_BLUE,
    RGB_MAX,
};

#define RGB_ON(fd) write(fd,"on",2)
#define RGB_OFF(fd) write(fd,"off",3)

App_Led::App_Led(QObject *parent)
    : App_Key{parent}
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    if(QFile::exists("/sys/clever/led/red/switch")) {        
        QtConcurrent::run(this,&App_Led::led_run);
        //QTimer::singleShot(1,this,SLOT(led_initFunSlot()));
    } else cout << "led err";
#endif
}

App_Led::~App_Led()
{
    int *rgb = mRgb; mLedIsRun = false;
    for(int i=0; i<RGB_MAX; i++) {
        if(rgb[i]) close(rgb[i]);
    }
}

void App_Led::led_initFun()
{
    led_factoryRestore();
    int *rgb = mRgb; mLedIsRun = true;
    rgb[RGB_RED] = open("/sys/clever/led/red/switch", O_RDWR);
    if(rgb[RGB_RED] < 0) perror("open red");
    rgb[RGB_GREEN] = open("/sys/clever/led/green/switch", O_RDWR);
    if(rgb[RGB_GREEN] < 0) perror("open green");
    rgb[RGB_BLUE] = open("/sys/clever/led/blue/switch", O_RDWR);
    if(rgb[RGB_BLUE] < 0) perror("open blue");
}


void App_Led::led_factoryRestore()
{
    QString fn = "/usr/data/clever/cfg/factory_restore.ini";
    QString dst = "/usr/data/clever/cfg/factory_restore.conf";
    if(QFile::exists(fn) || QFile::exists(dst)) m_fr = true; else m_fr = false;
}


void App_Led::led_delayOff()
{
    int *rgb = mRgb; int t = 500;
    int status = cm::masterDev()->status;
    //if(!status && !m_fr) t /= 5;
    if(status == 3) t /= 5;
    for(int i=0; i<t; ++i) {
        if(mLedIsRun) cm::mdelay(1);
    }

    for(int i=0; i<RGB_MAX; i++) {
        if(mLedIsRun) RGB_OFF(rgb[i]);
        cm::mdelay(1);
    }

    for(int i=0; i<t; ++i) {
        if(status == 3) cm::mdelay(1);
        else if(!status && !m_fr) cm::mdelay(1);
    }
}

void App_Led::led_workDown()
{
    sDevData *dev = cm::masterDev();
    int *rgb = mRgb; switch (dev->status) {
    case 2: RGB_ON(rgb[RGB_RED]); break;
    case 3: case 4: case 5: RGB_ON(rgb[RGB_BLUE]); break;
    case 1: RGB_ON(rgb[RGB_RED]); RGB_ON(rgb[RGB_GREEN]); RGB_ON(rgb[RGB_RED]); break;
    default: if(RGB_ON(rgb[RGB_GREEN]) < 0)  perror("RGB_GREEN on fail\n"); break;
    } led_delayOff(); //cout << dev->status;
}

void App_Led::led_run()
{
    cm::mdelay(10); led_initFun();
    while(mLedIsRun) led_workDown();
}
