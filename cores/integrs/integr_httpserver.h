#ifndef INTEGR_HTTPSERVER_H
#define INTEGR_HTTPSERVER_H
#include "httpserver.h"
#include "integr_pushthread.h"

class Integr_HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit Integr_HttpServer(QObject *parent = nullptr);
    static void initHttpServer(bool en, int port=23412);
    static void initHttpsServer(bool en, int port=23413);

private:
    static bool execute(const QByteArray &body);
    static bool download(const QByteArray &body);
    static bool pduMetaData(const QByteArray &body);
    static bool getDataItem(const QByteArray &body);
    static bool setDataItem(const QByteArray &body);
    static bool setCfgItem(const QByteArray &body);
    static bool getCfgItem(const QByteArray &body);
    static void replyValue(const QJsonValue &v);
    static bool replyHttp(const QString &msg, const int &httpStatusCode=200);
    static void onHttpAccepted(const QPointer<JQHttpServer::Session> &session);

private:
    static QPointer<JQHttpServer::Session> mSession;
};

#endif // INTEGR_HTTPSERVER_H
