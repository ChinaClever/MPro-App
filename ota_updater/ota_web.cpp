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

bool Ota_Web::web_readRsaSig(sOtaFile &it)
{
    QString fn = it.path+it.file;
    fn = fn.replace(".zip", ".sig");
    QFile file(fn); QString md5, sig;
    if(file.open(QIODevice::ReadOnly)) {
        md5 = file.readLine();
        sig = file.readLine();
        md5 = md5.left(md5.size() - 2);
        qDebug() << md5.size() << sig.size();
    } else { ota_error(11);
        throwMessage(tr("校验文件不存在 %1").arg(fn));
        file.close(); return false;
    } file.close();

    //it.md5.clear(); it.sig.clear();
    if(md5.size() == 32) {
        if(File::CheckMd5(it)) it.md5 = md5;
        else { ota_error(4);
            QString str = tr("MD5校验码错误:%1%2");
            throwMessage(str.arg(md5.size()).arg(md5));
            return false;
        }
    } else { ota_error(12);
        QString str = tr("MD5校验码格式不对:%1%2");
        throwMessage(str.arg(md5.size()).arg(md5));
        return false;
    }

    if(sig.size() > 256) it.sig = sig;
    else { ota_error(13);
        QString str = tr("签名信息格式不对:%1");
        throwMessage(str.arg(sig.data()));
        return false;
    }

    bool ok = sign_verify(it); QString msg = "RSA Verified ";
    if(ok) msg += "ok"; else {msg += "error"; ota_error(4);}
    throwMessage(msg);

    return ok;
}



void Ota_Web::web_workDown()
{
    if(mOta->web.isRun == 1) return;
    if(mOta->work & (1<<DOta_Web)) {
        QString dir = "/usr/data/upload/";
        QStringList fns = File::entryList(dir);
        foreach (const auto &fn, fns) {
            if((fn == ".") || (fn == "..") || fn.contains(".sig")) continue;

            sOtaFile it; it.fc = 1;
            it.file = fn; it.path = dir;
            it.size = File::size(dir+fn);
            bool ok = web_readRsaSig(it);
            //  it.md5 = fn.split(".").last();
            //  it.size = File::size(fn);
            //  if(it.md5 != 32) it.md5 = File::md5(fn);
            //  else ok = it.md5 == File::md5(fn);
            ota_updater(it, DOta_Web, ok);
        }
    }
}
