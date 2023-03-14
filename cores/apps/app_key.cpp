/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_key.h"
#include "set_core.h"

App_Key::App_Key(QObject *parent)
    : App_Buzzer{parent}
{
    QTimer::singleShot(500,this, &App_Key::key_read);
}


void App_Key::key_read()
{
    int res = cm::execute("cmd_gpio get_value PA15").toInt();
    if(0 == res) {
        if(mKeyCnt++ > 11) Set_Core::bulid()->factoryRestore();
        else QTimer::singleShot(500,this, &App_Key::key_read);
    } //cout << res;
}
