#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <QtCore>
#include <QSerialPort>
#include "commons.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    static QStringList ports();
    void closeSerial() {mSerial->close();}
    bool isOpened(){return mSerial->isOpen();}
    QString nameSerial() {return mSerial->portName();}
    bool isContains(const QString &name) {return ports().contains(name);}
    bool openSerial(const QString &name,qint32 baudRate = QSerialPort::Baud19200);

    int writeSerial(quint8 *cmd, int len);
    bool writeSerial(const QByteArray &array);
    QByteArray transmit(uchar *sent, int len, int msecs=1000);
    QByteArray transmit(const QByteArray &array, int msecs=1000);
    QByteArray readSerial(int msecs=1000);
    void setBaudRate(qint32 br);
    bool waitForLock();

public slots:
    void cmsWriteSlot(int msecs=1);
    void setBaudRateSlot() {mSerial->setBaudRate(mBr);}

private:
    bool isRun; qint32 mBr;
    QByteArrayList mList;
    QSerialPort  *mSerial;
    QReadWriteLock *mRwLock;
};

#endif // SERIALPORT_H
