#ifndef MQTT_SUBSCRIBER_H
#define MQTT_SUBSCRIBER_H

#include "mqtt_publisher.h"

class Mqtt_Subscriber : public Mqtt_Publisher
{
    Q_OBJECT
public:
    explicit Mqtt_Subscriber(QObject *parent = nullptr);

signals:

};

#endif // MQTT_SUBSCRIBER_H
