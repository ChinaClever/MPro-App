/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_integr.h"
#include "mb_core.h"
#include "agent_core.h"
#include "rpc_service.h"
#include "integr_core.h"

Set_Integr::Set_Integr()
{

}

int Set_Integr::modbusCfg(uchar fc)
{
    sModbusSetting *cfg = &(Mb_Core::modbusCfg);
    int res = 0; switch (fc) {
    case 1: res = cfg->enRtu; break;
    case 2: res = cfg->addr; break;
    case 3: res = baudTo(cfg->baud); break;
    case 4: res = parityTo(cfg->parity); break;
    case 5: res = dataBitsTo(cfg->dataBits); break;
    case 6: res = stopBitsTo(cfg->stopBits); break;
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
    case 3: key = "baud"; value = toBaud(value); mb->setRtu(2, value); break;
    case 4: key = "parity"; value = toParity(value); mb->setRtu(1, value); break;
    case 5: key = "dataBits"; value = toDataBits(value); mb->setRtu(3, value); break;
    case 6: key = "stopBits"; value = toStopBits(value); mb->setRtu(4, value); break;
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

QString Set_Integr::snmpCfg(uchar fc)
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QString res; switch (fc) {
    case 1: res = cfg->trap1; break;
    case 2: res = cfg->trap2; break;
    case 3: res = cfg->enV3; break;
    case 4: res = cfg->usr; break;
    case 5: res = cfg->pwd; break;
    case 6: res = cfg->key; break;
    default: qDebug() << Q_FUNC_INFO << fc; break;
    }

    return res;

}

bool Set_Integr::snmpSet(uchar fc, const QString &value, uchar txType)
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QString prefix = "snmp";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "trap1"; cfg->trap1 = value; break;
    case 2: key = "trap2"; cfg->trap2 = value; break;
    case 3: key = "enV3"; cfg->enV3 = value.toInt(); break;
    case 4: key = "usr"; cfg->usr = value; break;
    case 5: key = "pwd"; cfg->pwd = value; break;
    case 6: key = "key"; cfg->key = value; break;
    default: ret = false; qDebug() << Q_FUNC_INFO << fc; break;
    }

    if(ret) {
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, value, prefix);

        sOpItem db; db.op_src = opSrc(txType);
        db.content = QObject::tr("snmp %1参数修改为%2").arg(key, value);
        Log_Core::bulid()->append(db);
    }

    return ret;
}

int Set_Integr::rpcCfg(uchar fc)
{
    sRpcCfg *cfg = &(Rpc_Service::rpcCfg);
    int res = 0; switch (fc) {
    case 1: res = cfg->en; break;
    case 2: res = cfg->port; break;
    default: qDebug() << Q_FUNC_INFO << fc; break;
    }
    return res;
}

bool Set_Integr::rpcSet(uchar fc, int value, uchar txType)
{
    Rpc_Service *obj = Rpc_Service::bulid();
    QString prefix = "rpc";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "enRpc"; obj->startRpc(value); break;
    case 2: key = "port"; obj->setPort(value); break;
    default: ret = false; qDebug() << Q_FUNC_INFO << fc; break;
    }

    if(ret) {
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, value, prefix);

        sOpItem db; db.op_src = opSrc(txType);
        db.content = QObject::tr("rpc %1参数修改为%2").arg(key).arg(value);
        Log_Core::bulid()->append(db);
    }

    return ret;
}

QVariant Set_Integr::pushCfg(uchar fc)
{
    sPushCfg *cfg = &Integr_Core::pushCfg;
    QVariant res = 0; switch (fc) {
    case 1: res = cfg->udp[0].en; break;
    case 2: res = cfg->udp[0].host; break;
    case 3: res = cfg->udp[0].port; break;
    case 4: res = cfg->udp[1].en; break;
    case 5: res = cfg->udp[1].host; break;
    case 6: res = cfg->udp[1].port; break;
    case 7: res = cfg->recvEn; break;
    case 8: res = cfg->recvPort; break;
    case 9: res = cfg->sec; break;
    case 11: res = cfg->http.en; break;
    case 12: res = cfg->http.url; break;
    case 13: res = cfg->http.timeout; break;
    case 14: res = cfg->http.enServ; break;
    case 15: res = cfg->http.port; break;
    default: qDebug() << Q_FUNC_INFO << fc; break;
    }
    return res;
}


bool Set_Integr::pushSet(uchar fc, QVariant value, uchar txType)
{
    sPushCfg *cfg = &Integr_Core::pushCfg;
    Integr_Core *obj = Integr_Core::bulid();
    QString prefix = "push";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "udpEn"; cfg->udp[0].en = value.toInt(); break;
    case 2: key = "ddpHost"; cfg->udp[0].host = value.toString(); break;
    case 3: key = "udpPort"; cfg->udp[0].port = value.toInt(); break;
    case 4: key = "udp2En"; cfg->udp[1].en = value.toInt(); break;
    case 5: key = "ddp2Host"; cfg->udp[1].host = value.toString(); break;
    case 6: key = "udp2Port"; cfg->udp[1].port = value.toInt(); break;
    case 7: key = "recvEn"; obj->startRecv(value.toInt()); break;
    case 8: key = "recvPort"; obj->setRecvPort(value.toInt()); break;
    case 9: key = "sec"; cfg->sec = value.toInt(); break;

    case 11: key = "httpEn"; cfg->http.en = value.toInt(); break;
    case 12: key = "httpUrl"; cfg->http.url = value.toString(); ; break;
    case 13: key = "httpTimeout"; cfg->http.timeout = value.toInt(); break;
    case 14: key = "enServ"; cfg->http.enServ = value.toInt(); break;
    case 15: key = "httpPort"; cfg->http.port = value.toInt(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO << fc; break;
    }

    if(ret) {
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, value, prefix);

        sOpItem db; db.op_src = opSrc(txType);
        db.content = QObject::tr("push %1参数修改为%2").arg(key, value.toString());
        Log_Core::bulid()->append(db);
    }

    return ret;
}





int Set_Integr::baudTo(int value)
{
    int res = 0;
    switch(value) {
    case QSerialPort::Baud9600: res = 0; break;
    case QSerialPort::Baud19200: res = 1; break;
    case QSerialPort::Baud38400: res = 2; break;
    case QSerialPort::Baud57600: res = 3; break;
    case QSerialPort::Baud115200: res = 4; break;
    }
    return res;
}

int Set_Integr::toBaud(int value)
{
    int res = QSerialPort::Baud9600;
    switch(value) {
    case 0: res = QSerialPort::Baud9600; break;
    case 1: res = QSerialPort::Baud19200; break;
    case 2: res = QSerialPort::Baud38400; break;
    case 3: res = QSerialPort::Baud57600; break;
    case 4: res = QSerialPort::Baud115200; break;
    }
    return res;
}

int Set_Integr::parityTo(int value)
{
    int res = 0;
    switch(value) {
    case QSerialPort::NoParity: res = 0; break;
    case QSerialPort::EvenParity: res = 1; break;
    case QSerialPort::OddParity: res = 2; break;
    case QSerialPort::SpaceParity: res = 3; break;
    case QSerialPort::MarkParity: res = 4; break;
    }
    return res;
}

int Set_Integr::toParity(int value)
{
    int res = QSerialPort::NoParity;
    switch(value) {
    case 0: res = QSerialPort::NoParity; break;
    case 1: res = QSerialPort::EvenParity; break;
    case 2: res = QSerialPort::OddParity; break;
    case 3: res = QSerialPort::SpaceParity; break;
    case 4: res = QSerialPort::MarkParity; break;
    }
    return res;
}

int Set_Integr::dataBitsTo(int value)
{
    int res = 3;
    switch(value) {
    case QSerialPort::Data5: res = 0; break;
    case QSerialPort::Data6: res = 1; break;
    case QSerialPort::Data7: res = 2; break;
    case QSerialPort::Data8: res = 3; break;
    }
    return res;
}

int Set_Integr::toDataBits(int value)
{
    int res = QSerialPort::Data8;
    switch(value) {
    case 0: res = QSerialPort::Data5; break;
    case 1: res = QSerialPort::Data6; break;
    case 2: res = QSerialPort::Data7; break;
    case 3: res = QSerialPort::Data8; break;
    }
    return res;
}

int Set_Integr::stopBitsTo(int value)
{
    int res = 0;
    switch(value) {
    case QSerialPort::OneStop: res = 0; break;
    case QSerialPort::TwoStop: res = 1; break;
    case QSerialPort::OneAndHalfStop: res = 2; break;
    }
    return res;
}

int Set_Integr::toStopBits(int value)
{
    int res = QSerialPort::OneStop;
    switch(value) {
    case 0: res = QSerialPort::OneStop; break;
    case 1: res = QSerialPort::TwoStop; break;
    case 2: res = QSerialPort::OneAndHalfStop; break;
    }
    return res;
}
