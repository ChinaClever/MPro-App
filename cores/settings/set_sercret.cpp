/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_sercret.h"
#include "sercret_core.h"
#include "app_core.h"

Set_Sercret::Set_Sercret()
{

}


QVariant Set_Sercret::getSercret(uchar fc)
{
    Sercret_Core *it = Sercret_Core::bulid();
    QVariant res; switch (fc) {
    case 1: res = it->cfg.type; break;

    case 11: res = it->aesCfg.mode; break;
    case 12: res = it->aesCfg.padding; break;
    case 13: res = it->aesCfg.level; break;
    case 14: res = QString(it->aesCfg.key); break;
    case 15: res = QString(it->aesCfg.iv); break;

    case 21: res = it->rsaCfg.length; break;
    case 22: res = QString(it->rsaCfg.padding); break;
    case 23: res = QString(it->rsaCfg.pubKey); break;
    case 24: res = QString(it->rsaCfg.privKey); break;

    case 31: res = QString(it->sm4Cfg.type); break;
    case 32: res = QString(it->sm4Cfg.padding); break;
    case 33: res = QString(it->sm4Cfg.key); break;
    case 34: res = QString(it->sm4Cfg.iv); break;
    default: qDebug() << Q_FUNC_INFO << fc; break;
    }
    return res;
}


bool Set_Sercret::setSercret(uchar fc, const QVariant &v)
{
    bool ret = true;
    QString prefix = "sercret"; QString key;
    Sercret_Core *it = Sercret_Core::bulid();

    switch (fc) {
    case 1: key = "en";  it->cfg.type = v.toInt(); break;
    case 11: key = "aes_mode";  it->aes_setMode(v.toInt()); break;
    case 12: key = "aes_padding";  it->aes_setPadding(v.toInt());  break;
    case 13: key = "aes_level";  it->aes_setLevel(v.toInt());  break;
    case 14: key = "aes_key";  it->aesCfg.key = v.toByteArray(); break;
    case 15: key = "aes_iv";  it->aesCfg.iv = v.toByteArray(); break;
    case 33: key = "sm4_key"; it->sm4Cfg.key = v.toByteArray(); break;
    case 34: key = "sm4_iv"; it->sm4Cfg.iv = v.toByteArray(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(ret) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}

QVariant Set_Sercret::whiteListCfg(int fc)
{
    sWhiteListCfg *cfg = &App_WhiteList::whiteListCfg;
    QVariant res; switch (fc) {
    case 1: res = cfg->en; break;
    case 2: res = cfg->mac[0]; break;
    case 3: res = cfg->mac[1]; break;        
    case 4: res = cfg->mac[2]; break;
    case 5: res = cfg->mac[3]; break;
    case 6: res = cfg->ip[0]; break;
    case 7: res = cfg->ip[1]; break;
    case 8: res = cfg->ip[2]; break;
    case 9: res = cfg->ip[3]; break;
    default: cout << fc; break;
    }

    return res;
}

bool Set_Sercret::setWhiteList(uchar fc, const QVariant &v)
{
    bool ret = true;
    QString prefix = "whiteList"; QString key;
    sWhiteListCfg *cfg = &App_WhiteList::whiteListCfg;

    switch (fc) {
    case 1: key = "en";  cfg->en = v.toInt(); break;
    case 2: key = "mac1";  cfg->mac[0] = v.toString(); break;
    case 3: key = "mac2";  cfg->mac[1] = v.toString();  break;
    case 4: key = "mac3";  cfg->mac[2] = v.toString(); break;
    case 5: key = "mac4";  cfg->mac[3] = v.toString();  break;
    case 6: key = "ip1";  cfg->ip[0] = v.toString();  break;
    case 7: key = "ip2";  cfg->ip[1] = v.toString(); break;
    case 8: key = "ip3";  cfg->ip[2] = v.toString();  break;
    case 9: key = "ip4";  cfg->ip[3] = v.toString(); break;
    case 10: App_Core::bulid()->internetFirewall(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(ret) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}
