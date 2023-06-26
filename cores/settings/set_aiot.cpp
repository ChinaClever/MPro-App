#include "set_aiot.h"
#include "aiot/aiot_core.h"

Set_Aiot::Set_Aiot()
{

}


QVariant Set_Aiot::aiotCfg(int fc)
{
    sAiotCfg *it = &Aiot_LinkSdk::aiotCfg;
    Aiot_Core *obj = Aiot_Core::bulid();
    QVariant ret; switch (fc) {
    case 1: ret = it->en?1:0; break;
    case 2: ret = it->host; break;
    case 3: ret = it->product_key; break;
    case 4: ret = it->device_name; break;
    case 5: ret = it->device_secret; break;
    case 6: ret = it->time; break;
    case 7: ret = *it->connect; break;
    case 8: obj->setting(); break;
    default: cout << fc; break;
    }
    return ret;
}

bool Set_Aiot::aiotSet(int fc, const QVariant &v)
{
    bool ret = true;
    sAiotCfg *it = &Aiot_LinkSdk::aiotCfg;
    Aiot_Core *obj = Aiot_Core::bulid();
    QString prefix = "aiot"; QString key;

    switch (fc) {
    case 1: key = "en"; it->en = v.toInt(); break;
    case 2: key = "host";  it->host = v.toString(); break;
    case 3: key = "product_key";  it->product_key = v.toString();  break;
    case 4: key = "device_name";  it->device_name = v.toString();  break;
    case 5: key = "device_secret"; it->device_secret = v.toString();  break;
    case 6: key = "time"; it->time = v.toInt();  break;
    case 8: obj->setting(); break;
    default: ret = false; cout << fc; break;
    }

    if(key.size()){
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }
    return ret;
}
