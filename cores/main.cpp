#include <QCoreApplication>
#include "ipc_demo.h"
#include "db_user.h"
#include "modbus_slavetcp.h"
#include "ssdp_server.h"
#include "snmp/snmpagent.h"
#include "dtls_sender.h"
#include "cascade_slave.h"
#include "http/http.h"

#include "agent_devdata.h"
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

static void outputCtrl(QObject *p)
{
    OP_Core *core = OP_Core::bulid(p);


//    QString fn = "/home/lzy/ZMDPU_ZXB_APP_V1_6.bin.31313131313033383237";
//    qDebug() << "CCCCCCCCCCC" <<  core->ota_start(fn);

//    return ;

    core->setDelay(0, 0);

    cm::mdelay(500);

qDebug() << "XXXXXXXXX";
    core->orderCtrl(0);
    qDebug() << "JJJJJJJJJJJJJJ";
    cm::mdelay(500);

    for(int i=0; i<12; ++i)
    {
        cm::mdelay(500);
        core->relayCtrl(i+1, 1);
//        core->orderCtrl(1);
        cm::mdelay(500);
        core->relayCtrl(i+1, 0);
//        core->orderCtrl(1);
    }

    core->setDelay(0, 1);
//    cm::mdelay(500);
    core->orderCtrl(1);
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QObject *p = a.parent();
    outputCtrl(p);

    // ipc_demo(a.parent());
    // log_demo();
    // modbus_master();
    // modbus_slave();
    // ssdp_demo();
    // dtls_demo();
    // snmp_agent();
    // Cascade_Core::bulid();


//    op_Core();

//    OP_Core *op = new OP_Core();
    // Dtls_Recver::bulid();



    //     new c_sDevData;
    //    Cascade_Object obj;
    //    c_sDevData *data = obj.getDev();
    ////     = data;


    //    memset(data, 0, sizeof(c_sDevData));
    //    data->id = 10;

    //    data->lineSize = 2;
    //    strcpy(data->info.devName, "luozhiyong");
    //    strcpy(data->line[0].name, "1111");
    //    strcpy(data->line[1].name, "22222");

    //    QByteArray b = obj.toDataStream();
    //    memset(data, 0, sizeof(c_sDevData));

    //    c_sDevData *rcv = obj.deDataStream(b);
    //    qDebug() << rcv->id <<  rcv->info.devName <<  rcv->lineSize<<  rcv->line[1].name;


    //    c_sDevData data;

    //

    //    Cascade_DataStream s(&data);
    //    QByteArray b;
    //    QDataStream in(&b, QIODevice::WriteOnly);
    //    in << s;

    //    c_sDevData gdata;
    //    Cascade_DataStream g(&gdata);
    //    QDataStream out(&b, QIODevice::ReadOnly );
    //    out >> g;
    //

//    Agent_DevData *dev = new Agent_DevData();
//    dev->addOids();




    return a.exec();
}
