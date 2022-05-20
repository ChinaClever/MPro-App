#ifndef MODBUS_OBJECT_H
#define MODBUS_OBJECT_H
#include "serialport.h"
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#include <QModbusRtuSerialSlave>

struct sModbusSetting {
    QString portName;
    int parity = QSerialPort::EvenParity;
    int baud = QSerialPort::Baud19200;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int responseTime = 1000;
    int numberOfRetries = 3;
};
typedef QVector<ushort> vshort;

class Modbus_Object : public QObject
{
    Q_OBJECT
public:
    explicit Modbus_Object(QObject *parent = nullptr);
    ~Modbus_Object() {disconnectModbus();}
    virtual bool connectModbus(const QString &, int) = 0;
    void setRegisterType(QModbusDataUnit::RegisterType type) { m_type = type; }
    bool isConnectedModbus();
    void disconnectModbus();

protected:
    virtual QModbusDevice *modbusDevice() = 0;
    void initModbusNet(const QString &address, int port);
    void initModbusSerial(const QString &portName, qint32 baudRate);

    bool connectDevice();
    bool waitForState(int state);
    bool replyFinished(QModbusReply *reply);
    void throwError(const QString &err);

protected slots:
    void onStateChanged(int state);
    void handleDeviceError(QModbusDevice::Error newError);

protected:
    sModbusSetting mSet;
    QModbusDataUnit::RegisterType m_type;
};

#endif // MODBUS_OBJECT_H
