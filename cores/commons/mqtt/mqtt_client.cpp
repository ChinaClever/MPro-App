#include "mqtt_client.h"
#include "sercet_tlscert.h"

sMqttCfg Mqtt_Client::cfg;
Mqtt_Client::Mqtt_Client(QObject *parent)
    : QObject{parent}
{
//    connectToHost();

//    _timer.start(1000);
//    connect(&_timer, &QTimer::timeout, this, &Mqtt_Client::onTimeout);
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
        ret = false; if(m_client) m_client->disconnectFromHost();
    }

    if(ret) {
        connect(m_client, &QMQTT::Client::subscribed, this, &Mqtt_Client::onSubscribed);
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
        m_client->setVersion(QMQTT::MQTTVersion::V3_1_1);
        m_client->connectToHost();
    }
}

void Mqtt_Client::onConnected()
{
    cfg.isConnected = true;
    QString topic = "pduSetting/" + cfg.clientId;
    m_client->subscribe(topic, cfg.qos);

    m_client->subscribe("testtopic/#", cfg.qos);

}

void Mqtt_Client::onSubscribed(const QString& topic)
{
    qDebug() << "subscribed " << topic ;
}

bool Mqtt_Client::publish(const QByteArray &payload)
{
    bool ret = false;
    if(cfg.isConnected) {
        QString topic = "pduMetaData/"+ cfg.clientId;
        QMQTT::Message message(_number++, topic, payload, cfg.qos);
        ret = m_client->publish(message);

        qDebug() << "a" << (quint16)ret;
        QString topic2 = "pduSetting/"+ cfg.clientId;
        QMQTT::Message message2(_number++, topic2, payload, cfg.qos);
        ret = m_client->publish(message2);
        qDebug() << "b" << (quint16)ret;

    }
    return ret;
}

void Mqtt_Client::onReceived(const QMQTT::Message& message)
{
    emit received(message.payload());
    qDebug() <<message.topic() << message.payload();
}
