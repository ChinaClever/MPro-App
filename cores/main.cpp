#include <QCoreApplication>
#include "ipc_demo.h"
#include "db_user.h"
#include "modbus_slavetcp.h"
#include "ssdp_server.h"
#include "snmp/snmpagent.h"
#include "dtls_sender.h"

void log_demo()
{
    Db_User *db = Db_User::bulid();

    sUserItem it;
    it.name = "lzy";
    it.pwd = "123456";
    it.email = "luozhiyong131@qq.com";
    it.telephone = "18576689472";
    db->insertItem(it);

    QVector<sUserItem> its = db->selectAll();
    qDebug() << db->jsonWriteFile(db->toJson(its));

    db->clear();
    //    qDebug() << db->minId() << db->maxId();
    //    db->removeItemsByName(it.name);
    //    qDebug() << db->selectBetween(8,10).size();
}

void modbus_master()
{
    Modbus_MasterTcp *tcp = new Modbus_MasterTcp();
    tcp->connectModbus("192.168.31.52", 502);

    QVector<quint16> rcv{9,8,7,6,5};
    qDebug() <<  tcp->writeHoldingRegisters(1, 0, rcv);

    QVector<quint16> data =  tcp->readHoldingRegisters(1, 0, 5);
    qDebug() << "recv" << data;
}

void modbus_slave()
{
    Modbus_SlaveTcp *tcp = new Modbus_SlaveTcp();
    qDebug() << tcp->connectTcp( 1502, "192.168.31.14");
    //tcp->setData(QModbusDataUnit::HoldingRegisters, 0, 10);

    QVector<quint16> data{9,8,7,6,5};
    tcp->setHoldingRegisters(1, data);
}

bool snmp_callback(uint id, const QString &oid, const QVariant &v)
{
    qDebug() << "snmp callback" << id << oid << v;
    return true;
}

int snmp_value = 55;

void snmp_agent()
{
    SnmpModule *snmp = SnmpAgent::bulid();
    sOidIt it;

    it.name = "test";
    it.fieldId = 1;
    it.oid = "10.3.2";
    it.intPtr = &snmp_value;
    it.callback = snmp_callback;

    bool ret = snmp->addOid(it);
    qDebug() << "snmp agent add oid" << ret;
}

void ssdp_demo()
{

    Ssdp_Client *c = Ssdp_Client::bulid();

    Ssdp_Server *s = Ssdp_Server::bulid();
    qDebug() << s->searchTarget();
    s->write("lzyluo");

}

void dtls_demo()
{
    Dtls_Recver *ser = new Dtls_Recver;
    ser->setFile("lzy.txt");

    QStringList ips {"127.0.0.1"};
    QByteArray array{"012345678901234567890123456789"};

    Dtls_Sender *c = new Dtls_Sender;
    c->send(ips, array);

    qDebug() << ser->waitForFinish();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // ipc_demo(a.parent());
    // log_demo();
    // modbus_master();
    // modbus_slave();
    // ssdp_demo();
    // dtls_demo();
    // snmp_agent();


    return a.exec();
}
