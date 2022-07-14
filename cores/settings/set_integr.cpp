#include "set_integr.h"
#include "mb_core.h"

Set_Integr::Set_Integr()
{

}

int Set_Integr::modbusCfg(uchar fc)
{
    sModbusSetting *cfg = Mb_Core::bulid()->cfg();
    int res = 0; switch (fc) {
    case 1: res = cfg->enRtu; break;
    case 2: res = cfg->addr; break;
    case 3: res = cfg->baud; break;
    case 4: res = cfg->parity; break;
    case 5: res = cfg->dataBits; break;
    case 6: res = cfg->stopBits; break;
    case 11: res = cfg->enTcp; break;
    case 12: res = cfg->port; break;
    default: qDebug() << Q_FUNC_INFO << fc; break;
    }

    return res;
}

bool Set_Integr::modbusSet(uchar fc, int value, uchar txType)
{
    Mb_Core *mb = Mb_Core::bulid();
    QString prefix = "modbus";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "enRtu"; mb->connectRtu(value); break;
    case 2: key = "addr"; mb->setAddress(value); break;
    case 3: key = "baud"; mb->setRtu(2, value); break;
    case 4: key = "parity"; mb->setRtu(1, value); break;
    case 5: key = "dataBits"; mb->setRtu(3, value); break;
    case 6: key = "stopBits"; mb->setRtu(4, value); break;
    case 11: key = "enTcp"; mb->connectTcp(value); break;
    case 12: key = "port"; mb->setPort(value); break;
    default: ret = false; qDebug() << Q_FUNC_INFO << fc; break;
    }

    if(ret) {
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, value, prefix);

        sOpItem db; db.op_src = opSrc(txType);
        db.content = QObject::tr("Modbus %1参数修改为%2").arg(key).arg(value);
        Log_Core::bulid()->append(db);
    }

    return ret;
}
