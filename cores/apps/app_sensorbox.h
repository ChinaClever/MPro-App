#ifndef APP_SENSORBOX_H
#define APP_SENSORBOX_H
#include "app_sensor.h"
#include "serialport.h"

class App_SensorBox : public App_Sensor
{
public:
    explicit App_SensorBox(QObject *parent = nullptr);
    ~App_SensorBox();
    void sensorBox_run();

private:
    bool box_open();
    void box_close();
    bool box_readData();
    ushort getShort(uchar *ptr);
    bool box_recvPacket(const QByteArray &array);

private:
    bool box_isRun=true;
    SerialPort *mSerial=nullptr;
};

#endif // APP_SENSORBOX_H
