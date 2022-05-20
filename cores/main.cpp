#include <QCoreApplication>
#include "ipc_coreserver.h"
#include "ssdp_server.h"
#include "http/http.h"
#include "app_start.h"

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
    App_Start::bulid(p);





    return a.exec();
}
