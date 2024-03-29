#ifndef INTEGR_HTTPSERVER_H
#define INTEGR_HTTPSERVER_H
#include "httpserver.h"
#include "integr_pushthread.h"

class Integr_HttpServer : public QObject
{
public:
    explicit Integr_HttpServer(QObject *parent = nullptr);
    static void initHttpServer(bool en, int port=23412);
    static void initHttpsServer(bool en, int port=23413);

private:
    static bool upload(const QByteArray &body);
    static bool execute(const QByteArray &body);
    static bool download(const QByteArray &body);
    static bool pduMetaData(const QByteArray &body);
    static bool getting(const QByteArray &body);
    static bool setting(const QByteArray &body);
    static bool relayCtrl(const QByteArray &body);
    static void replyValue(const QJsonValue &v);
    static bool replyHttp(const QString &msg, const int &httpStatusCode=200);
    static void onHttpAccepted(const QPointer<JQHttpServer::Session> &session);

private:
    static QPointer<JQHttpServer::Session> mSession;
};

#endif // INTEGR_HTTPSERVER_H
