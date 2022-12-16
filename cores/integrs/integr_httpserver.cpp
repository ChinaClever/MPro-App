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
    int addr = it->getData(body, "id");
    if(addr >= 0) {
        QJsonObject obj = Integr_JsonBuild::bulid()->getJsonObject(addr);
        mSession->replyJsonObject(obj);
    } else {
        ret = replyHttp("error", 211);
    }
    return ret;
}

bool Integr_HttpServer::getDataItem(const QByteArray &body)
{
    QJsonObject object;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    bool ret = it->checkInput(body, object);
    if(ret) {
        double res = it->getDataItem(object);
        replyValue(res);
    } else {
        ret = replyHttp("error", 212);
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
        ret = replyHttp(str+file, 214);
    }
    return ret;
}

void Integr_HttpServer::OnGetHeaders(const QByteArray &body)
{
    QString str = "form-data; name=";
    int id = body.indexOf(str) + str.size()+1;
    int idx = body.indexOf("filename=") - 2;

    cout << id << idx << idx-id << body.mid(id, idx-id);


//    if(body.contains("Content-Disposition")) {

//        QString strFileName = m_reply->rawHeader("Content-Disposition");
//        int idx = strFileName.indexOf(QStringLiteral("filename="));
//        if(idx >= 0) //找到filename字段
//        {
//            strFileName = strFileName.replace('"', ""); //filename=后面有双引号，应该全部删除
//            strFileName = strFileName.mid(idx + 9);
//            m_strFileName = strFileName;
//        }
//    }
//    else //如果没有Content-Disposition，则用QUrl获取basename
//    {
//        m_url = m_reply->url();
//        if(!m_url.isEmpty())
//        {
//            m_strFileName = m_url.fileName();
//        }
//    }

//    if(m_reply->hasRawHeader("Content-Length"))
//    {
//        QString strFileSize = m_reply->rawHeader("Content-Length");
//        m_strFileSize = strFileSize;
//    }
//    else
//    {
//        m_strFileSize = "0";
//    }
}

bool Integr_HttpServer::upload(const QByteArray &body)
{
        qDebug() << "AAAAAAAAAAAAAA" << body;
    OnGetHeaders(body);
        replyHttp("ok", 200);
//    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
//    QString file = it->getString(body, "file");
//    if(file.size()) {
//        QJsonObject object; QByteArray array;
//        bool ret = it->checkInput(body, object);
//        if(ret) array = it->getValue(object, "content").toArray();

//    }
        return true;
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

bool Integr_HttpServer::getCfgItem(const QByteArray &body)
{
    QJsonObject object;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    bool ret = it->checkInput(body, object);
    if(ret) {
        QVariant res = it->getCfgItem(object);
        replyValue(res.toJsonValue());
    } else {
        ret = replyHttp("error", 213);
    }
    return ret;
}

bool Integr_HttpServer::setCfgItem(const QByteArray &body)
{
    QJsonObject object;
    Integr_JsonRecv *it = Integr_JsonRecv::bulid();
    bool ret = it->checkInput(body, object);
    if(ret) ret = it->setCfgItem(object);
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
        else if(url.contains("getDataItem")) getDataItem(body);
        else if(url.contains("getCfgItem")) getCfgItem(body);
        else if(url.contains("download")) download(body);
        else replyHttp(err, 213);
    } else if(method.contains("POST")) {
        if(url.contains("setDataItem")) setDataItem(body);
        else if(url.contains("setCfgItem")) setCfgItem(body);
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
