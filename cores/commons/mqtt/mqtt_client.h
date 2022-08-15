#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include <QTimer>
//#include <QObject>
//#include <QMqttClient>
//#include <QtMqtt/qmqttclient.h>
#include "qmqtt/qmqtt.h"

struct sMqttCfg {
    sMqttCfg():type(1), url("192.168.1.102"), port(1883),path("/mqtt"),
        clientId("clientId"), isConnected(false), keepAlive(60), qos(0){}

    int type; // 1 mqtt 2 mqtts 3 ws 4 wss
    QString url;
    quint16 port;
    QString path;
    QString clientId;
    QByteArray usr, pwd;
    bool isConnected;
    int keepAlive;
    int qos;

};

class Mqtt_Client : public QObject
{
    Q_OBJECT
public:
    explicit Mqtt_Client(QObject *parent = nullptr);
    static sMqttCfg cfg;
    ~Mqtt_Client();

    void connectToHost();
    bool publish(const QByteArray &payload);
    void subscribe(const QString& topic, const quint8 qos = 0);

signals:
    void received(const QByteArray &payload);

private slots:

    void onTimeout()
       {
        publish("AAAAAAAAAAAA");

    }
    void onConnected();
    void onSubscribed(const QString& topic);
    void onDisconnected(){cfg.isConnected = false;}
    void onReceived(const QMQTT::Message& message);
    void onError(const QMQTT::ClientError error) {qDebug() << Q_FUNC_INFO << error;}

private:
    bool createMqtt();

private:
    quint16 _number=0;
     QTimer _timer;
    QMQTT::Client *m_client;
};

#endif // MQTT_CLIENT_H
