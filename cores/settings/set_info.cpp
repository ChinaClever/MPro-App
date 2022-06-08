#include "set_info.h"
#include "cfg_obj.h"

Set_Info::Set_Info()
{

}

QString Set_Info::getUut(int addr, uchar fc)
{
    char *ptr=nullptr;
    sUutInfo *it = &(cm::devData(addr)->uut);

    switch (fc) {
    case 1: ptr = it->idc; break;
    case 2: ptr = it->room; break;
    case 3: ptr = it->module; break;
    case 4: ptr = it->cab; break;
    case 5: ptr = it->road; break;
    case 6: ptr = it->devName; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}

bool Set_Info::setUut(uchar fc, char *str, uchar txType)
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

    qstrcpy(ptr, str);
    Cfg_Obj *cfg = Cfg_Obj::bulid(CFG_FN);
    cfg->writeCfg(key, QString(ptr), prefix);

    sOpItem db; db.op_src = opSrc(txType);
    db.content = QObject::tr("%1 修改为 %2").arg(key).arg(str);
    Log_Core::bulid()->append(db);

    return ret;
}
