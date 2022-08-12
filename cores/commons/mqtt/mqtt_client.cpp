#include "mqtt_client.h"
#include "file.h"
#include <QSslKey>

sMqttCfg Mqtt_Client::cfg;
Mqtt_Client::Mqtt_Client(QObject *parent)
    : QObject{parent}
{
    connectToHost();
}

Mqtt_Client::~Mqtt_Client()
{
    if(m_client) m_client->disconnectFromHost();
}

QSslConfiguration Mqtt_Client::sslConfiguration()
{
    QSslConfiguration ssl;
    QFile certFile(File::certFile());
    QFile keyFile(File::keyFile());

    bool ret = keyFile.open(QIODevice::ReadOnly);
    if(ret) ret = certFile.open(QIODevice::ReadOnly);
    if(ret) {
        QSslCertificate certificate(&certFile, QSsl::Pem);
        QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
        ssl.setPeerVerifyMode(QSslSocket::VerifyNone);
        ssl.setLocalCertificate(certificate);
        ssl.setPrivateKey(sslKey);
    }
    return ssl;
}

bool Mqtt_Client::createMqtt()
{
    bool ret = true;
    if(1 == cfg.type) {
        QHostAddress host(cfg.url);
        m_client = new QMQTT::Client(host, cfg.port, this);
    } else if(2 == cfg.type) {
        QSslConfiguration ssl = sslConfiguration();
        m_client = new QMQTT::Client(cfg.url, cfg.port, ssl, false, this);
    } else if(3 == cfg.type) {
        QString cmd = "ws://%1:%2%3";
        QString url = cmd.arg(cfg.url).arg(cfg.port).arg(cfg.path);
        m_client = new QMQTT::Client(url, "", QWebSocketProtocol::VersionLatest, false, this);
    } else if(4 == cfg.type) {
        QString cmd = "wss://%1:%2%3";
        QString url = cmd.arg(cfg.url).arg(cfg.port).arg(cfg.path);
        QSslConfiguration ssl = sslConfiguration();
        m_client = new QMQTT::Client(url, "", QWebSocketProtocol::VersionLatest, ssl, false, this);
    } else {
        ret = false; if(m_client) m_client->disconnectFromHost();
    }

    if(ret) {
        connect(m_client, &QMQTT::Client::error, this, &Mqtt_Client::onError);
        connect(m_client, &QMQTT::Client::connected, this, &Mqtt_Client::onConnected);
        connect(m_client, &QMQTT::Client::disconnected, this, &Mqtt_Client::onDisconnected);
    }
    return ret;
}

void Mqtt_Client::connectToHost()
{
    bool ret = createMqtt();
    if(ret) {
        m_client->setClientId(cfg.clientId);
        m_client->setAutoReconnect(true);
        m_client->setCleanSession(true);
        m_client->setUsername(cfg.usr);
        m_client->setPassword(cfg.pwd);
        m_client->connectToHost();
    }
}


void Mqtt_Client::onConnected()
{
    cfg.isConnected = true;
    qDebug() << "AAAAAAAAAAAAAAA";
}

void Mqtt_Client::onDisconnected()
{
    cfg.isConnected = false;
}

void Mqtt_Client::onError(const QMQTT::ClientError error)
{
    qDebug() << "BBBBBBBBBBBBBB" << error;
}
