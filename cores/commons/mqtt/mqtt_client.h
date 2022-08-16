#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include "qmqtt/qmqtt.h"

struct sMqttCfg {
    sMqttCfg():type(1), url("127.0.0.1"), port(1883), path("/mqtt"),
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
    explicit Mqtt_Client(QObject *parent = nullptr);
public:
    static Mqtt_Client *bulid(QObject *parent = nullptr);
    bool publish(const QByteArray &payload);
    bool set(uchar fc, const QVariant &v);
    static sMqttCfg cfg;
    ~Mqtt_Client();

signals:
    void received(const QByteArray &payload);

private slots:
    void onConnected();
    void onDisconnected(){cfg.isConnected = false;}
    void onReceived(const QMQTT::Message& message);
    void onError(const QMQTT::ClientError error) {qDebug() << Q_FUNC_INFO << error;}

private:
    void subscribe();
    bool createMqtt();
    void connectToHost();

private:
    quint16 m_number=0;
    QMQTT::Client *m_client=nullptr;
};

#endif // MQTT_CLIENT_H
