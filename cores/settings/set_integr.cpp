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
#include "mqtt_client.h"
#include "qrabbitmq.h"
#include "odbc_obj.h"
#include "redis_core.h"

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
    default: cout << fc; break;
    }

    return res;
}

bool Set_Integr::modbusSet(uchar fc, int value)
{
    sModbusSetting *cfg = &(Mb_Core::modbusCfg);
    QString prefix = "modbus";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "enRtu"; cfg->enRtu = value; break;
    case 2: key = "addr"; cfg->addr = value; cm::masterDev()->cfg.param.modbusRtuAddr = value; break;
    case 3: key = "baud"; value = toBaud(value); cm::masterDev()->cfg.param.modbusRtuBr = cfg->baud = value; break;
    case 4: key = "parity"; value = toParity(value); cfg->parity = value;  /*mb->setRtu(1, value);*/ break;
    case 5: key = "dataBits"; value = toDataBits(value); cfg->dataBits = value; /*mb->setRtu(3, value);*/ break;
    case 6: key = "stopBits"; value = toStopBits(value); cfg->stopBits = value; /*mb->setRtu(4, value);*/ break;
    case 7: emit Mb_Core::bulid()->connectRtuSig(); break;

    case 11: key = "enTcp"; cfg->enTcp = value; break;
    case 12: key = "port"; cfg->port = value; break;
    case 13: emit Mb_Core::bulid()->connectTcpSig(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, value, prefix);
    }

    return ret;
}

QVariant Set_Integr::snmpCfg(uchar fc, int id)
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QVariant res; switch (fc) {
    case 1: res = cfg->enV2; break;
    case 11: res = cfg->enV3; break;
    case 12: res = cfg->usr; break;
    case 13: res = cfg->pwd; break;
    case 14: res = cfg->key; break;
    case 15: res = cfg->encrypt; break;
    case 21: res = cfg->trap[id]; break;
    case 22: res = cfg->inform[id]; break;
    default: cout << fc; break;
    }

    return res;
}

bool Set_Integr::snmpSet(uchar fc, int id, const QVariant &v)
{
    sAgentCfg *cfg = &(Agent_Core::snmpCfg);
    QString prefix = "snmp";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "enV2"; cfg->enV2 = v.toInt(); break;
    case 11: key = "enV3"; cfg->enV3 = v.toInt(); break;
    case 12: key = "usr"; cfg->usr = v.toString(); break;
    case 13: key = "pwd"; cfg->pwd = v.toString(); break;
    case 14: key = "key"; cfg->key = v.toString(); break;
    case 15: key = "encrypt"; cfg->encrypt = v.toInt(); break;
    case 17: Agent_Core::bulid()->set_snmpdV3(); break;
    case 21: key = "trap_" + QString::number(id); cfg->trap[id] = v.toString(); break;
    case 22: key = "inform_" + QString::number(id); cfg->inform[id] = v.toString(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }

    return ret;
}

int Set_Integr::rpcCfg(uchar fc)
{
    sRpcCfg *cfg = &(Rpc_Service::rpcCfg);
    int res = 0; switch (fc) {
    case 1: res = cfg->json.en; break;
    case 2: res = cfg->json.port; break;
    case 4: res = cfg->xml.en; break;
    case 5: res = cfg->xml.port; break;
    default: cout << fc; break;
    }

    return res;
}

bool Set_Integr::rpcSet(uchar fc, int value)
{
    sRpcCfg *cfg = &(Rpc_Service::rpcCfg);
    QString prefix = "rpc";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "jsonRpcEn"; cfg->json.en = value; break;
    case 2: key = "jsonRpcPort"; cfg->json.port = value; break;
    case 3: ret = Rpc_Service::bulid()->startJsonRpc(); break;

    case 4: key = "xmlRpcEn"; cfg->xml.en = value; break;
    case 5: key = "xmlRpcPort"; cfg->xml.port = value; break;
    case 6: ret = Rpc_Service::bulid()->startXmlRpc(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, value, prefix);
    }

    return ret;
}

QVariant Set_Integr::mqttCfg(uchar fc)
{
    sMqttCfg *cfg = &(Mqtt_Client::cfg);
    QVariant res = 0; switch (fc) {
    case 1: res = cfg->type; break;
    case 2: res = cfg->url; break;
    case 3: res = cfg->port; break;
    case 4: res = cfg->path; break;
    case 5: res = cfg->clientId; break;
    case 6: res = QString(cfg->usr); break;
    case 7: res = QString(cfg->pwd); break;
    case 8: res = cfg->keepAlive; break;
    case 9: res = cfg->qos; break;
    case 10: res = cfg->isConnected; break;
    case 11: res = cfg->sec; break;
    default: cout << fc; break;
    }
    return res;
}

bool Set_Integr::mqttSet(uchar fc, const QVariant &v)
{
    sMqttCfg *cfg = &(Mqtt_Client::cfg);
    QString prefix = "mqtt";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "type"; cfg->type = v.toInt(); break;
    case 2: key = "url"; cfg->url = v.toString(); break;
    case 3: key = "port"; cfg->port = v.toInt(); break;
    case 4: key = "path"; cfg->path = v.toString(); break;
    case 5: key = "clientId";  cfg->clientId = v.toString();break;
    case 6: key = "usr"; cfg->usr = v.toString().toLatin1(); break;
    case 7: key = "pwd";  cfg->pwd = v.toString().toLatin1(); break;
    case 8: key = "keepAlive"; cfg->keepAlive = v.toInt(); break;
    case 9: key = "qos"; cfg->qos = v.toInt(); break;
    case 11: key = "sec"; cfg->sec = v.toInt(); break;
    case 20: Mqtt_Client::bulid()->startMqtt(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
        //Mqtt_Client::bulid()->set(fc, v);
    }

    return ret;
}


QVariant Set_Integr::amqpCfg(uchar fc)
{
    sAmqpCfg *cfg = &QRabbitMQ::amqpCfg;
    QVariant res = 0; switch (fc) {
    case 1: res = cfg->en; break;
    case 2: res = cfg->host; break;
    case 3: res = cfg->port; break;
    case 4: res = cfg->virtualHost; break;
    case 5: res = cfg->username; break;
    case 6: res = cfg->password; break;
    case 7: res = cfg->name; break;
    case 8: res = cfg->routingKey; break;
    case 9: res = cfg->bindingKey; break;
    case 10: res = cfg->ssl; break;
    case 11: res = cfg->isConnected; break;
    case 12: res = cfg->sec; break;
    default: cout << fc; break;
    }
    return res;
}


bool Set_Integr::amqpSet(uchar fc, const QVariant &v)
{
    sAmqpCfg *cfg = &(QRabbitMQ::amqpCfg);
    QString prefix = "amqp";  QString key;

    bool ret = true; switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 2: key = "host"; cfg->host = v.toString(); break;
    case 3: key = "port"; cfg->port = v.toInt(); break;
    case 4: key = "virtualHost"; cfg->virtualHost =v.toString(); break;
    case 5: key = "username"; cfg->username = v.toString(); break;
    case 6: key = "password"; cfg->password = v.toString(); break;
    case 7: key = "name"; cfg->name = v.toString(); break;
    case 8: key = "routingKey"; cfg->routingKey = v.toString(); break;
    case 9: key = "bindingKey"; cfg->bindingKey = v.toString(); break;
    case 10: key = "ssl"; cfg->ssl = v.toInt(); break;
    case 12: key = "sec"; cfg->sec = v.toInt(); break;
    case 20: QRabbitMQ::bulid()->start(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }

    return ret;
}

QVariant Set_Integr::redisCfg(uchar fc)
{
    sRedisCfg *cfg = &Redis_Obj::redisCfg;
    QVariant res = 0; switch (fc) {
    case 1: res = cfg->en; break;
    case 2: res = cfg->host; break;
    case 3: res = cfg->port; break;
    case 4: res = cfg->pwd; break;
    case 5: res = cfg->db; break;
    case 6: res = cfg->key; break;
    case 7: res = cfg->subscribe; break;
    case 8: res = cfg->sec; break;
    case 9: res = cfg->alive; break;
    case 10: res = cfg->isConnect?1:0; break;
    case 11: res = cfg->error; break;
    default: cout << fc; break;
    }
    return res;
}


bool Set_Integr::redisSet(uchar fc, const QVariant &v)
{
    sRedisCfg *cfg = &Redis_Obj::redisCfg;
    QString prefix = "redis";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 2: key = "host"; cfg->host = v.toString(); break;
    case 3: key = "port"; cfg->port = v.toInt(); break;
    case 4: key = "pwd"; cfg->pwd = v.toString(); break;
    case 5: key = "db"; cfg->db =v.toInt(); break;
    case 6: key = "pdukey"; cfg->key = v.toByteArray(); break;
    case 7: key = "subscribe"; cfg->subscribe = v.toByteArray(); break;
    case 8: key = "sec"; cfg->sec = v.toInt(); break;
    case 9: key = "alive"; cfg->alive = v.toInt(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }

    return ret;
}



QVariant Set_Integr::odbcCfg(uchar fc)
{
    sOdbcCfg *cfg = &Odbc_Obj::cfg;
    QVariant res = 0; switch (fc) {
    case 1: res = cfg->en; break;
    case 2: res = cfg->host; break;
    case 3: res = cfg->port; break;        
    case 4: res = cfg->user; break;
    case 5: res = cfg->pwd; break;
    case 6: res = cfg->db; break;
    case 7: res = cfg->pdukey; break;
    case 8: res = cfg->dataPoll; break;
    case 9: res = cfg->hdaPoll; break;
    case 10: res = cfg->status?1:0; break;
    case 11: res = cfg->okCnt; break;
    case 12: res = cfg->errCnt; break;
    default: cout << fc; break;
    }
    return res;
}

bool Set_Integr::odbcSet(uchar fc, const QVariant &v)
{
    sOdbcCfg *cfg = &(Odbc_Obj::cfg);
    QString prefix = "odbc";  QString key;
    bool ret = true; switch (fc) {
    case 1: key = "en"; cfg->en = v.toInt(); break;
    case 2: key = "host"; cfg->host = v.toString(); break;
    case 3: key = "port"; cfg->port = v.toInt(); break;
    case 4: key = "user"; cfg->user = v.toString(); break;
    case 5: key = "pwd"; cfg->pwd = v.toString(); break;
    case 6: key = "db"; cfg->db =v.toString(); break;
    case 7: key = "pdukey"; cfg->pdukey = v.toString(); break;
    case 8: key = "dataPoll"; cfg->dataPoll = v.toInt(); break;
    case 9: key = "hdaPoll"; cfg->hdaPoll = v.toInt(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
    }

    return ret;
}


QVariant Set_Integr::pushCfg(uchar fc, int id)
{
    sPushCfg *cfg = &Integr_Core::pushCfg;
    QVariant res = 0; if(id) id -= 1; switch (fc) {
    case 1: res = cfg->udp[id].en; break;
    case 2: res = cfg->udp[id].host; break;
    case 3: res = cfg->udp[id].port; break;
    case 4: res = cfg->udp[id].sec; break;
    case 8: res = cfg->recvEn; break;
    case 9: res = cfg->recvPort; break;

    case 11: res = cfg->http.en; break;
    case 12: res = cfg->http.url; break;
    case 13: res = cfg->http.timeout; break;
    case 15: res = cfg->http.enServer; break;
    case 16: res = cfg->http.port; break;
    case 14: res = cfg->http.sec; break;
    default: cout << fc; break;
    }

    return res;
}


bool Set_Integr::pushSet(uchar fc, int id, const QVariant &v)
{
    sPushCfg *cfg = &Integr_Core::pushCfg;
    QString prefix = "push";  QString key;
    bool ret = true; if(id) id -= 1; switch (fc) {
    case 1: key = "udpEn_" + QString::number(id); cfg->udp[id].en = v.toInt(); break;
    case 2: key = "udpHost_" + QString::number(id); cfg->udp[id].host = v.toString(); break;
    case 3: key = "udpPort_" + QString::number(id); cfg->udp[id].port = v.toInt(); break;
    case 4: key = "udpSec_"+ QString::number(id); cfg->udp[id].sec = v.toInt(); break;
    case 8: key = "recvEn"; cfg->recvEn = v.toInt(); break;
    case 9: key = "recvPort"; cfg->recvPort = v.toInt(); break;
    case 10: Integr_Core::bulid()->initRecvFun(); break;

        // case 10: key = "dc";  cfg->dataContent = v.toInt(); break;
    case 11: key = "httpEn"; cfg->http.en = v.toInt(); break;
    case 12: key = "httpUrl"; cfg->http.url = v.toString(); ; break;
    case 13: key = "httpTimeout"; cfg->http.timeout = v.toInt(); break;
    case 15: key = "enServer"; cfg->http.enServer = v.toInt(); break;
    case 16: key = "httpPort"; cfg->http.port = v.toInt(); break;
    case 14: key = "httpSec"; cfg->http.sec = v.toInt(); break;
    case 19: Integr_Core::bulid()->httpServer(); break;
    default: ret = false; cout << fc; break;
    }

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key, v, prefix);
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
