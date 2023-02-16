#ifndef SERIALPORT_H
#define SERIALPORT_H
//#include <QtCore>
#include <QSerialPort>
#include "commons.h"
#include "serial_port.h"

class SerialPort : public QObject
{
public:
    explicit SerialPort(QObject *parent = nullptr);
    static QStringList ports();
    void closeSerial() {mSerial->closePort();}
    bool isOpened(){return mSerial->isOpen();}
    QString nameSerial() {return mSerial->portName();}
    bool isContains(const QString &name) {return ports().contains(name);}
    bool openSerial(const QString &name, qint32 baudRate = QSerialPort::Baud19200,
                    QSerialPort::Parity parity = QSerialPort::NoParity);

    int writeSerial(quint8 *cmd, int len);
    bool writeSerial(const QByteArray &array);
    QByteArray transmit(uchar *sent, int len, int msecs=1000);
    QByteArray transmit(const QByteArray &array, int msecs=1000);
    QByteArray readSerial(int msecs=1000);
    bool hasCmdWrite(){return mCmdList.size();}
    void cmsWrite(int msecs=1);
    bool waitForLock();

private:
    void waitForSend(int size);

private:
    qint32 mBr=1;
    Serial_Port  *mSerial;
    QReadWriteLock *mRwLock;
    QByteArrayList mCmdList;
};

#endif // SERIALPORT_H
