#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <QObject>
//#include <QMqttClient>
#include <QtMqtt/qmqttclient.h>

class Mqtt_Client : public QObject
{
    Q_OBJECT
public:
    explicit Mqtt_Client(QObject *parent = nullptr);

signals:

private:
     QMqttClient *m_client;
};

#endif // MQTT_CLIENT_H
