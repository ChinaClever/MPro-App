#include "set_info.h"
#include "cfg_obj.h"

Set_Info::Set_Info()
{

}

bool Set_Info::setUut(uchar fc, char *str, uchar rw)
{
    bool ret = true;
    QString prefix = "uut";
    QString key; char *ptr=nullptr;
    sUutInfo *it = &(cm::masterDev()->uut);

    switch (fc) {
    case 1: key = "idc";  ptr = it->idc; break;
    case 2: key = "room";  ptr = it->room; break;
    case 3: key = "module";  ptr = it->module; break;
    case 4: key = "cab";  ptr = it->cab; break;
    case 5: key = "road";  ptr = it->road; break;
    case 6: key = "devName";  ptr = it->devName; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(rw) {
        Cfg_Obj *cfg = Cfg_Obj::bulid(CFG_FN);
        cfg->writeCfg(key, QString(ptr), prefix);
        qstrcpy(ptr, str);
    } else {
        qstrcpy(str, ptr);
    }

    return ret;
}
