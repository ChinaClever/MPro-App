#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include "mqtt_client.h"

class Mqtt_Publisher : public Mqtt_Client
{
    Q_OBJECT
public:
    explicit Mqtt_Publisher(QObject *parent = nullptr);


private:
};

#endif // MQTT_PUBLISHER_H
