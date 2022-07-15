#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <JQHttpServer>

class HttpServer : public QObject
{
    Q_OBJECT
public:
    explicit HttpServer(QObject *parent = nullptr);
    static void initHttpServer(int port=23412);
    static void initHttpsServer(int port=23413);

private:
    static void onHttpAccepted(const QPointer<JQHttpServer::Session> &session);
};

#endif // HTTPSERVER_H
