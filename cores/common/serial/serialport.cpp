#include "serialport.h"
#include <QSerialPortInfo>

SerialPort::SerialPort(QObject *parent)
    : QObject{parent}
{
    mSerial = new QSerialPort(this);
    mRwLock = new QReadWriteLock;
}

bool SerialPort::openSerial(const QString &name,qint32 baudRate)
{
    mSerial->close(); mSerial->setPortName(name);
    bool ret = mSerial->open(QIODevice::ReadWrite);
    if(ret) {
        mSerial->setBaudRate(baudRate);  //波特率
        mSerial->setDataBits(QSerialPort::Data8); //数据位
        mSerial->setParity(QSerialPort::NoParity);    //无奇偶校验
        mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
        mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
    } else qDebug() << mSerial->errorString();

    return ret;
}

QStringList SerialPort::ports()
{
    QStringList list;
    QList<QSerialPortInfo>  infos = QSerialPortInfo::availablePorts();
    foreach (auto &info, infos)  list <<  info.portName();
    return list;
}

bool SerialPort::waitForLock()
{
    bool ret = false;
    do {
        ret = mRwLock->tryLockForWrite();
        if(ret) break; else mdelay(1);
    }while(!ret);
    mRwLock->unlock();
    return ret;
}

int SerialPort::writeSerial(const QByteArray &array)
{
    QWriteLocker locker(mRwLock);
    int ret = mSerial->write(array);
    if(ret > 0) {mSerial->flush(); mdelay(5);}
    else qDebug() << mSerial->errorString();
    return ret;
}

QByteArray SerialPort::readSerial(int msecs)
{
    QByteArray rcv;
    QWriteLocker locker(mRwLock);
    for(int i=0; i<msecs; i+=10) {
        mdelay(10); rcv =  mSerial->readAll();
        if(rcv.size()) break;
    }

    return rcv;
}

QByteArray SerialPort::transmit(const QByteArray &array, int msecs)
{
    QByteArray rcv;
    if(writeSerial(array) > 0) {
        rcv = readSerial(msecs);
    }
    return rcv;
}

QByteArray SerialPort::transmit(uchar *sent, int len, int msecs)
{
    return transmit(QByteArray((char *)sent, len), msecs);
}

int SerialPort::writeSerial(quint8 *cmd, int len)
{
    return writeSerial(QByteArray((char *)cmd, len));
}
