/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_httpserver.h"

QPointer<JQHttpServer::Session> Integr_HttpServer::mSession;
Integr_HttpServer::Integr_HttpServer(QObject *parent)
    : QObject{parent}
{

}

bool Integr_HttpServer::replyHttp(const QString &msg, const int &httpStatusCode)
{
    bool ret = false;
    QJsonObject json;
    json.insert("message", msg);
    mSession->replyJsonObject(json, httpStatusCode);
    if(httpStatusCode == 200) ret = true;
    return ret;
}


bool Integr_HttpServer::pduMetaData(const QByteArray &body)
{
    bool ret = true;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    int addr = it->getData(body, "id");
    if(addr >= 0) {
        QJsonObject obj = Integr_JsonBuild::bulid()->getJsonObject(addr);
        mSession->replyJsonObject(obj);
    } else {
        ret = replyHttp("error", 211);
    }
    return ret;
}

bool Integr_HttpServer::download(const QByteArray &body)
{
    bool ret = true;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    QString file = it->getString(body, "file");
    if(QFile::exists(file)) {
        mSession->replyFile(file);
    } else {
        QString str = "error: file does not exist, ";
        ret = replyHttp(str+file, 212);
    }
    return ret;
}

bool Integr_HttpServer::setDataItem(const QByteArray &body)
{
    QJsonObject object;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    bool ret = it->checkInput(body, object);
    if(ret) ret = it->setDataItem(object);
    if(ret) replyHttp("ok", 200);
    else replyHttp("error", 221);
    return ret;
}


bool Integr_HttpServer::setNumStrItem(const QByteArray &body)
{
    QJsonObject object;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    bool ret = it->checkInput(body, object);
    if(ret) ret = it->setNumStrItem(object);
    if(ret) replyHttp("ok", 200);
    else replyHttp("error", 222);
    return ret;
}


bool Integr_HttpServer::execute(const QByteArray &body)
{
    bool ret = true;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    QString cmd = it->getString(body, "cmd");
    if(cmd.size()) {
        replyHttp("ok", 200);
        system(cmd.toLatin1().data());
    } else {
        ret = replyHttp("error:" + cmd, 231);
    }
    return ret;
}


void Integr_HttpServer::onHttpAccepted(const QPointer<JQHttpServer::Session> &session)
{
    QString err = "error: Interface not supported";
    QByteArray body = session->requestBody();
    QString method = session->requestMethod();
    QString url = session->requestUrl();
    mSession = session;

    if(method.contains("GET")) {
        if(url.contains("pduMetaData")) pduMetaData(body);
        else if(url.contains("download")) download(body);
        else replyHttp(err, 213);
    } else if(method.contains("POST")) {
        if(url.contains("setDataItem")) setDataItem(body);
        else if(url.contains("setNumStrItem")) setNumStrItem(body);
        else replyHttp(err, 223);
    } else if(method.contains("PUT")) {
        if(url.contains("execute")) execute(body);
        else replyHttp(err, 233);
    }
}

void Integr_HttpServer::initHttpServer(int port)
{
    static JQHttpServer::TcpServerManage tcpServerManage(1); // 设置最大处理线程数，默认2个
    tcpServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ));
    const auto listenSucceed = tcpServerManage.listen(QHostAddress::Any, port);
    qDebug() << "HTTP server listen:" << port << listenSucceed;
}

void Integr_HttpServer::initHttpsServer(int port)
{
#ifndef QT_NO_SSL
    static JQHttpServer::SslServerManage sslServerManage(1); // 设置最大处理线程数，默认2个
    sslServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ) );
    const auto listenSucceed = sslServerManage.listen( QHostAddress::Any, port,":/server.crt", ":/server.key" );
    qDebug() << "HTTPS server listen:" << port << listenSucceed;
#else
    qDebug() << "SSL not support"
#endif
}