/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mqtt_client.h"
#include "sercet_tlscert.h"
#include "commons.h"

sMqttCfg Mqtt_Client::cfg;
Mqtt_Client::Mqtt_Client(QObject *parent)
    : QObject{parent}
{
    startMqtt();
    connect(this, &Mqtt_Client::publish, this, &Mqtt_Client::onPublish);
}

Mqtt_Client *Mqtt_Client::bulid(QObject *parent)
{
    static Mqtt_Client* sington = nullptr;
    if(sington == nullptr) {
        sington = new Mqtt_Client(parent);
    }
    return sington;
}

Mqtt_Client::~Mqtt_Client()
{
    if(m_client) m_client->disconnectFromHost();
}

bool Mqtt_Client::createMqtt()
{
    bool ret = true;    
    if(1 == cfg.type) {
        QHostAddress host(cfg.url);
        m_client = new QMQTT::Client(host, cfg.port, this);
    } else if(2 == cfg.type) {
        QSslConfiguration ssl = Sercret_TlsCert::bulid()->sslConfiguration();
        m_client = new QMQTT::Client(cfg.url, cfg.port, ssl, false, this);
    } else if(3 == cfg.type) {
        QString cmd = "ws://%1:%2%3";
        QString url = cmd.arg(cfg.url).arg(cfg.port).arg(cfg.path);
        m_client = new QMQTT::Client(url, "", QWebSocketProtocol::VersionLatest, false, this);
    } else if(4 == cfg.type) {
        QString cmd = "wss://%1:%2%3";
        QString url = cmd.arg(cfg.url).arg(cfg.port).arg(cfg.path);
        QSslConfiguration ssl = Sercret_TlsCert::bulid()->sslConfiguration();
        m_client = new QMQTT::Client(url, "", QWebSocketProtocol::VersionLatest, ssl, false, this);
    } else {
        ret = false;
    }

    if(ret) {
        connect(m_client, &QMQTT::Client::error, this, &Mqtt_Client::onError);
        connect(m_client, &QMQTT::Client::received, this, &Mqtt_Client::onReceived);
        connect(m_client, &QMQTT::Client::connected, this, &Mqtt_Client::onConnected);
        connect(m_client, &QMQTT::Client::disconnected, this, &Mqtt_Client::onDisconnected);
    }
    return ret;
}

void Mqtt_Client::connectToHost()
{
    bool ret = createMqtt();
    if(ret) {
        m_client->setKeepAlive(cfg.keepAlive);
        m_client->setClientId(cfg.clientId);
        m_client->setAutoReconnect(true);
        m_client->setCleanSession(true);
        m_client->setUsername(cfg.usr);
        m_client->setPassword(cfg.pwd);
        m_client->connectToHost();
    } else if(m_client) m_client->setAutoReconnect(ret);
}

void Mqtt_Client::startMqtt()
{
    if(m_client) {
        m_client->disconnectFromHost();
        delete m_client; m_client=nullptr;
    }

    //cfg.url = "192.168.1.100";   ///////======
    //cfg.type = 0;
    if(cfg.type) connectToHost();

}
void Mqtt_Client::onConnected()
{
    cfg.isConnected = true;
    QString topic = "pduSetting/#";// + cfg.clientId;
    m_client->subscribe(topic, cfg.qos);
}

void Mqtt_Client::onPublish(const QByteArray &payload)
{
    if(cfg.isConnected && cfg.clientId.size()) {
        QString topic = "pduMetaData/"+ cfg.clientId;
        QMQTT::Message message(m_number++, topic, payload, cfg.qos);
        m_client->publish(message);
    }
}

void Mqtt_Client::onReceived(const QMQTT::Message& message)
{
    QString room = cm::masterDev()->cfg.uut.room;
    QString topic = message.topic().remove("pduSetting/");
    if((topic == cfg.clientId) || (topic == room) || topic == "all") {
        emit received(message.payload());
    }
    //qDebug() << "publish received: \""<< message.topic() << message.payload();
}

bool Mqtt_Client::set(uchar fc, const QVariant &v)
{
    bool ret = true; switch (fc) {
    case 1: cfg.type = v.toInt(); connectToHost(); break;
    case 2: cfg.url = v.toString(); if(m_client)m_client->setHostName(cfg.url); break;
    case 3: cfg.port = v.toInt(); if(m_client)m_client->setPort(cfg.port); break;
    case 4: cfg.path = v.toString(); connectToHost(); break;
    case 5: cfg.clientId = v.toString(); if(m_client)m_client->setClientId(cfg.clientId); break;
    case 6: cfg.usr = v.toByteArray();  if(m_client)m_client->setUsername(cfg.usr); break;
    case 7: cfg.pwd = v.toByteArray();  if(m_client)m_client->setPassword(cfg.pwd); break;
    case 8: cfg.keepAlive = v.toInt();  if(m_client)m_client->setKeepAlive(cfg.keepAlive); break;
    case 9: cfg.qos = v.toInt(); break;
    default: ret = false; qDebug() << Q_FUNC_INFO << fc << v; break;
    }
    return ret;
}
