#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

//#include <QObject>
//#include <QMqttClient>
//#include <QtMqtt/qmqttclient.h>
#include "qmqtt/qmqtt.h"

struct sMqttCfg {
    sMqttCfg():type(4), url("192.168.1.102"), port(8084),path("/mqtt"),
        clientId("clientId"), isConnected(false){}

    int type; // 1 mqtt 2 mqtts 3 ws 4 wss
    QString url;
    quint16 port;
    QString path;
    QString clientId;
    QByteArray usr, pwd;
    bool isConnected;

};

class Mqtt_Client : public QObject
{
    Q_OBJECT
public:
    explicit Mqtt_Client(QObject *parent = nullptr);
    static sMqttCfg cfg;
    ~Mqtt_Client();

public slots:
    void connectToHost();

private slots:
    void onConnected();
    void onDisconnected();
    void onError(const QMQTT::ClientError error);

private:
    bool createMqtt();
    QSslConfiguration sslConfiguration();

private:
    QMQTT::Client *m_client;
};

#endif // MQTT_CLIENT_H
