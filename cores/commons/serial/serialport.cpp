#include "serialport.h"
#include <QSerialPortInfo>

SerialPort::SerialPort(QObject *parent)
    : QObject{parent}
{
    isRun = false;
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
    } else qCritical() << mSerial->errorString();

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
        if(ret) break; else cm::mdelay(1);
    }while(!ret);
    mRwLock->unlock();
    return ret;
}

bool SerialPort::writeSerial(const QByteArray &array)
{
    mList << array;
    if(!isRun) {isRun = true; QTimer::singleShot(1,this, SLOT(writeSlot()));}
    return mSerial->isWritable();
}

void SerialPort::writeSlot()
{
    QWriteLocker locker(mRwLock);
    while(!mList.isEmpty()) {
        int ret = mSerial->write(mList.takeFirst());
        if(ret > 0) {mSerial->flush(); cm::mdelay(11);}
        else qCritical() << mSerial->errorString();
    } isRun = false;
}

QByteArray SerialPort::readSerial(int msecs)
{
    QByteArray rcv, array;
    bool ret = mSerial->waitForReadyRead(msecs);
    if(!ret) return rcv;

    do{
        cm::mdelay(100);
        array = mSerial->readAll();
        rcv.append(array);
    } while (array.size());

    return rcv;
}

QByteArray SerialPort::transmit(const QByteArray &array, int msecs)
{
    QByteArray rcv;
    QWriteLocker locker(mRwLock);
    if(mSerial->write(array) > 0) {
        mSerial->flush(); //mSerial->waitForBytesWritten();
        rcv = readSerial(msecs);
    } else qCritical() << mSerial->errorString();
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
