/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "httpserver.h"
#include <QJsonObject>
#include "cfg_file.h"

HttpServer::HttpServer(QObject *parent)
    : QObject{parent}
{

}

// 回调发生在专用的处理线程内，不是主线程，也不是socket的生存线程，请注意线程安全
// 若阻塞了此回调，那么新的连接将被阻塞（默认情况下有2个线程可以阻塞2次，第3个连接将被阻塞）
void HttpServer::onHttpAccepted(const QPointer<JQHttpServer::Session> &session)
{
    session->replyText(QString("url:%1\nbody:%2" ).arg(session->requestUrl(), QString(session->requestBody() ) ) );
//    session->replyRedirects( QUrl( "http://www.baidu.com" ) );
//    session->replyJsonObject( { { { "message", "ok" } } } );
//    session->replyJsonArray( { "a", "b", "c" } );
//    session->replyFile( "/Users/jason/Desktop/Test1.Test2" );
//    session->replyImage( QImage( "/Users/jason/Desktop/Test.png" ) );
//    session->replyBytes( QByteArray( 4,'\x24' ), "text" ); // $$$$

    // 注1：因为一个session对应一个单一的HTTP请求，所以session只能reply一次
    // 注2：在reply后，session的生命周期不可控，所以reply后不要再调用session的接口了
}


void HttpServer::initHttpServer(int port)
{
    static JQHttpServer::TcpServerManage tcpServerManage(1); // 设置最大处理线程数，默认2个
    tcpServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ));
    const auto listenSucceed = tcpServerManage.listen(QHostAddress::Any, port);
    qDebug() << "HTTP server listen:" << port << listenSucceed;
}

void HttpServer::initHttpsServer(int port)
{
#ifndef QT_NO_SSL
    static JQHttpServer::SslServerManage sslServerManage(1); // 设置最大处理线程数，默认2个
    sslServerManage.setHttpAcceptedCallback(std::bind(onHttpAccepted, std::placeholders::_1 ) );
    const auto listenSucceed = sslServerManage.listen( QHostAddress::Any, port,
                                                       File::certFile(), File::keyFile()
                                                       // ":/server.crt", ":/server.key"
                                                       );
    qDebug() << "HTTPS server listen:" << port << listenSucceed;

    // 这是我在一个实际项目中用的配置（用认证的证书，访问时带绿色小锁），其中涉及到隐私的细节已经被替换，但是任然能够看出整体用法
    /*
    qDebug() << "listen:" << sslServerManage.listen(
                    QHostAddress::Any,
                    24684,
                    "xxx/__xxx_com.crt",
                    "xxx/__xxx_com.key",
                    {
                        { "xxx/__xxx_com.ca-bundle", QSsl::Pem },
                        { "xxx/COMODO RSA Certification Authority.crt", QSsl::Pem },
                        { "xxx/COMODO RSA Domain Validation Secure Server CA.cer", QSsl::Der }
                    }
                );
    */
#else
    qDebug() << "SSL not support"
#endif
}
