/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_web.h"

Ota_Web::Ota_Web(QObject *parent)
    : Ota_Usb{parent}
{

}

void Ota_Web::web_workDown()
{
    if(mOta->web.isRun == 1) return;
    if(mOta->work & (1<<DOta_Web)) {
        QString dir = "/usr/data/clever/upload/";
        QStringList fns = File::entryList(dir);
        foreach (const auto &fn, fns) {
            if((fn == ".") || (fn == "..")) continue;

            bool ok = true;
            sOtaFile it; it.fc = 1;
            it.file = fn; it.path = dir;
            it.md5 = fn.split(".").last();
            it.size = File::size(fn);
            if(it.md5 != 32) it.md5 = File::md5(fn);
            else ok = it.md5 == File::md5(fn);
            ota_updater(it, DOta_Web, ok);
        }
    }
}
