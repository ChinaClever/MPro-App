/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_httpserver.h"
#include "cfg_file.h"

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

void Integr_HttpServer::replyValue(const QJsonValue &v)
{
    QJsonObject json;
    json.insert("value", v);
    mSession->replyJsonObject(json, 200);
}

bool Integr_HttpServer::pduMetaData(const QByteArray &body)
{
    bool ret = true;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    int addr = it->getData(body, "addr");
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
    QString fn = it->getString(body, "file");
    if(fn.at(0) != '/') fn.insert(0, '/');
    if(QFile::exists(fn)) {
        mSession->replyFile(fn);
    } else {
        QString str = "error: file does not exist, ";
        ret = replyHttp(str+fn, 214);
    }
    return ret;
}


bool Integr_HttpServer::upload(const QByteArray &body)
{
    bool ret = true;
    if(body.contains("Content-Disposition")) {
        QString str = "form-data; name=";
        int id = body.indexOf(str) + str.size()+1;
        int idx = body.indexOf("filename=") - 2;
        QString fn = body.mid(id, idx-id);
        str = QStringLiteral(";filename=\"%1\"\r\n\r\n").arg(fn.split("/").last());
        id = body.indexOf(str) + str.size();
        idx = body.indexOf("\r\n--boundary_.oOo._");
        QByteArray array = body.mid(id, idx-id);
        QString cmd = "rm -f " + fn;
        system(cmd.toLocal8Bit().data());
        if(fn.at(0) != '/') {fn.insert(0, '/');} QFile file(fn);
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.write(array); file.close();
        } replyHttp("ok:"+fn, 200);
    } else ret = replyHttp("error", 223);

    return ret;
}

bool Integr_HttpServer::getting(const QByteArray &body)
{
    QVariant res = Integr_JsonRecv::bulid()->reply(body);
    if(res.isNull()) replyHttp("error", 212);
    else replyValue(res.toJsonValue());
    return true;
}

bool Integr_HttpServer::setting(const QByteArray &body)
{
    bool ret = Integr_JsonRecv::bulid()->recv(body);
    if(ret) replyHttp("ok", 200); else replyHttp("error", 221);
    return ret;
}

bool Integr_HttpServer::execute(const QByteArray &body)
{
    bool ret = true;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    QString cmd = it->getString(body, "cmd");
    if(cmd.size()) {
        //system(cmd.toLatin1().data());
        QString res = cm::execute(cmd);
        replyHttp(res, 200);
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
        else if(url.contains("pduGetting")) getting(body);
        else if(url.contains("download")) download(body);
        else replyHttp(err, 213);
    } else if(method.contains("POST")) {
        if(url.contains("pduSetting")) setting(body);
        else if(url.contains("upload")) upload(body);
        else replyHttp(err, 223);
    } else if(method.contains("PUT")) {
        if(url.contains("execute")) execute(body);
        else replyHttp(err, 233);
    }
}

void Integr_HttpServer::initHttpServer(bool en, int port)
{
    static JQHttpServer::TcpServerManage *tcpServerManage=nullptr; // 设置最大处理线程数，默认2个
    if(tcpServerManage) {delete tcpServerManage; tcpServerManage=nullptr;}
    if(en) {
        tcpServerManage = new JQHttpServer::TcpServerManage(1);
        tcpServerManage->setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ));
        const auto listenSucceed = tcpServerManage->listen(QHostAddress::Any, port);
        qDebug() << "HTTP server listen:" << port << listenSucceed;
    }
}

void Integr_HttpServer::initHttpsServer(bool en, int port)
{
#ifndef QT_NO_SSL
    static JQHttpServer::SslServerManage *sslServerManage=nullptr; // 设置最大处理线程数，默认2个
    if(sslServerManage) {delete sslServerManage; sslServerManage=nullptr;}
    if(en) {
        sslServerManage = new JQHttpServer::SslServerManage(1);
        sslServerManage->setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ) );
        const auto listenSucceed = sslServerManage->listen( QHostAddress::Any, port,
                                                            File::certFile(), File::keyFile()
                                                            /*":/server.crt", ":/server.key"*/ );
        qDebug() << "HTTPS server listen:" << port << listenSucceed;
    }
#else
    qDebug() << "SSL not support";
#endif
}
