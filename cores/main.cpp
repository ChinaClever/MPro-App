#include <QCoreApplication>
#include "ipc_coreserver.h"
#include "ipc_alarmclient.h"

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
    IPC_CoreServer::bulid(p);
    //Dtls_Recver::bulid(p)->listen();
    //Cascade_Core *c = Cascade_Core::bulid(p);
    // Set_Core *set = Set_Core::bulid();
    OP_Core::bulid(p);

    cm::mdelay(100);

    IPC_RelayClient::bulid(p)->ctrl(0, 1,1);



    return a.exec();
}
