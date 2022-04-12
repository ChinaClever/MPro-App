#include <QCoreApplication>
#include "ipc_demo.h"
#include "db_user.h"
#include "modbus_slavetcp.h"
#include "ssdp_server.h"
//#include "snmp/snmpagent.h"
#include "dtls_sender.h"
#include "cascade_slave.h"
#include "http/http.h"

//#include "agent_devdata.h"
#include "op_zrtu.h"


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
    tcp->connectModbus("192.168.31.52", 1502);

    QVector<quint16> rcv{9,8,7,6,5};
    qDebug() <<  tcp->writeHoldingRegisters(1, 0, rcv);

    QVector<quint16> data =  tcp->readHoldingRegisters(1, 0, 5);
    qDebug() << "recv" << data;
}

void modbus_slave()
{
    Modbus_SlaveTcp *tcp = new Modbus_SlaveTcp();
    qDebug() << tcp->connectTcp(1502);
    //tcp->setData(QModbusDataUnit::HoldingRegisters, 0, 10);

    QVector<quint16> data{9,8,7,6,5};
    tcp->setHoldingRegisters(1, data);
}


void ssdp_demo()
{

    Ssdp_Client *c = Ssdp_Client::bulid();

    Ssdp_Server *s = Ssdp_Server::bulid();
    qDebug() << s->searchTarget();
    s->write("lzyluo");

}


void http_demo()
{
    // https://github.com/flaviotordini/http
    //    auto reply = Http::instance().post("https://google.com/", "LZY", "application/json");
    //    connect(reply, &HttpReply::finished, this, [](auto &reply) {
    //        if (reply.isSuccessful()) {
    //            qDebug() << "Feel the bytes!" << reply.body();
    //        } else {
    //            qDebug() << "Something's wrong here" << reply.statusCode() << reply.reasonPhrase();
    //        }
    //    });
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QObject *p = a.parent();

    Cascade_Core *c = Cascade_Core::bulid(p);
#if defined(Q_OS_LINUX)
    OP_Core * op = OP_Core::bulid(p);


#else
    c->setAddress(0);
    for(int i=0; i<10; ++i) {
        c->masterRelayCtrl(1, i+1, 0);
        cm::mdelay(500);
        c->masterRelayCtrl(1, i+1, 1);
        cm::mdelay(500);
    }

#endif



    return a.exec();
}
