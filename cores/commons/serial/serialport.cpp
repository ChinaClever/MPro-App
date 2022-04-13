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
    int cnt = 1;
    bool ret = false;
    do {
        ret = mRwLock->tryLockForWrite();
        if(ret) break; else cm::mdelay(1);
        if(0 == (cnt++ % 3000)) qDebug() << "Error: SerialPort waitForLock" << cnt;
    }while(!ret);
    mRwLock->unlock();
    return ret;
}

bool SerialPort::writeSerial(const QByteArray &array)
{
    mList << array;
    return mSerial->isWritable();
}

void SerialPort::cmsWriteSlot(int msecs)
{
    QWriteLocker locker(mRwLock);  while(mList.size()) {
        cm::mdelay(msecs); int ret = mSerial->write(mList.takeFirst());
        if(ret > 0) mSerial->flush(); else qCritical() << "Error" << mSerial->errorString();
        if(!mList.size()) cm::mdelay(2*msecs);
    }
}

QByteArray SerialPort::readSerial(int msecs)
{
    QByteArray rcv, array;
    mSerial->waitForReadyRead(msecs);
    do{
        cm::mdelay(msecs/10);
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
        mSerial->readAll(); mSerial->flush();
        rcv = readSerial(msecs);  //mSerial->waitForBytesWritten();
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
