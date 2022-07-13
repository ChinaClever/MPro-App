#include "mqtt_client.h"

Mqtt_Client::Mqtt_Client(QObject *parent)
    : QObject{parent}
{
    m_client = new QMqttClient(this);
}
