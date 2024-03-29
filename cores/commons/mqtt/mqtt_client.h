#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#include "qmqtt/qmqtt.h"

struct sMqttCfg {
    sMqttCfg():type(2), url("192.168.1.102"), port(8883), path("/mqtt"),
        clientId("clientId"), isConnected(false), keepAlive(60), qos(0), sec(5){}

    int type; // 1 mqtt 2 mqtts 3 ws 4 wss
    QString url;
    quint16 port;
    QString path;
    QString clientId;
    QByteArray usr, pwd;
    bool isConnected;
    int keepAlive;
    int qos;
    int sec;
};

class Mqtt_Client : public QObject
{
    Q_OBJECT
    explicit Mqtt_Client(QObject *parent = nullptr);
public:
    static Mqtt_Client *bulid(QObject *parent = nullptr);
    bool set(uchar fc, const QVariant &v);
    static sMqttCfg cfg;
    void startMqtt();
    ~Mqtt_Client();

signals:
    void publish(int addr, const QByteArray &payload);
    void received(const QByteArray &payload);

private slots:
    void onConnected();
    void onPublish(int addr, const QByteArray &payload);
    void onDisconnected();//{cfg.isConnected = false;}
    void onReceived(const QMQTT::Message& message);
    void onError(const QMQTT::ClientError error) {qDebug() << Q_FUNC_INFO << error;}

private:
    void subscribe();
    bool createMqtt();
    void connectToHost();
    bool uuidCheck(const QString &topic);

private:
    quint16 m_number=0;
    QMQTT::Client *m_client=nullptr;
};

#endif // MQTT_CLIENT_H
