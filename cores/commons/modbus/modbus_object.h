#ifndef MODBUS_OBJECT_H
#define MODBUS_OBJECT_H
#include "serialport.h"
#include <QModbusTcpClient>
#include <QModbusTcpServer>
#include <QModbusRtuSerialSlave>
#include <QHostAddress>

#define MB_NAME  "/dev/ttyUSB0"

struct sModbusSetting {
    int enRtu = false;
    QString portName = MB_NAME;
    int parity = QSerialPort::NoParity;
    int baud = QSerialPort::Baud9600;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int addr = 1;

    int enTcp = false;
    QHostAddress host = QHostAddress::Any;
    int responseTime = 1000;
    int numberOfRetries = 3;
    int port = 502;
};
typedef QVector<ushort> vshort;

class Modbus_Object : public QObject
{
    Q_OBJECT
public:
    explicit Modbus_Object(QObject *parent = nullptr);
    ~Modbus_Object() {disconnectModbus();}
    void setRegisterType(QModbusDataUnit::RegisterType type) { m_type = type; }
    void setModbus(int parameter, const QVariant &value);
    bool isConnectedModbus();
    void disconnectModbus();

protected:
    virtual QModbusDevice *modbusDevice() = 0;
    void initModbusSerial(const sModbusSetting &set);
    void initModbusNet(int port, const QString &addr="");

    bool connectDevice();
    bool waitForState(int state);
    bool replyFinished(QModbusReply *reply);
    void throwError(const QString &err);

protected slots:
    void onStateChanged(int state);
    void handleDeviceError(QModbusDevice::Error newError);

protected:
    //sModbusSetting mSet;
    QModbusDataUnit::RegisterType m_type;
};

#endif // MODBUS_OBJECT_H
