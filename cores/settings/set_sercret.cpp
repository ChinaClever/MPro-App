#include "set_sercret.h"
#include "sercret_core.h"

Set_Sercret::Set_Sercret()
{

}

QVariant Set_Sercret::getSercret(uchar fc)
{
    Sercret_Core * s = Sercret_Core::bulid();
    QVariant res; switch (fc) {
    case 1: res = s->cfg.type; break;

    case 11: res = s->aesCfg.mode; break;
    case 12: res = s->aesCfg.padding; break;
    case 13: res = s->aesCfg.level; break;
    case 14: res = s->aesCfg.key; break;
    case 15: res = s->aesCfg.iv; break;

    case 21: res = s->rsaCfg.pubKey; break;


    default: qDebug() << Q_FUNC_INFO << fc; break;
    }
    return res;
}
