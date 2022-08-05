/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "serialport.h"
#include <QSerialPortInfo>

SerialPort::SerialPort(QObject *parent)
    : QObject{parent}
{
    isRun = false;
    mSerial = new QSerialPort(this);
    mRwLock = new QReadWriteLock;
}

bool SerialPort::openSerial(const QString &name,qint32 baudRate, QSerialPort::Parity parity)
{
    mSerial->close(); mSerial->setPortName(name);
    bool ret = mSerial->open(QIODevice::ReadWrite);
    if(ret) {
        mSerial->setParity(parity);    //无奇偶校验
        mSerial->setBaudRate(baudRate);  //波特率
        mSerial->setDataBits(QSerialPort::Data8); //数据位        
        mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
        mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制
    } else qCritical() << Q_FUNC_INFO << mSerial->errorString();

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
    int cnt = 1; bool ret = false;
    if(!isOpened()) return ret;
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
    bool ret = mSerial->isWritable();
    if(ret) mList << array;
    return ret;
}

void SerialPort::cmsWriteSlot(int msecs)
{
    QWriteLocker locker(mRwLock);  while(mList.size()) {
        cm::mdelay(msecs); int ret = mSerial->write(mList.takeFirst());
        if(ret > 0) mSerial->flush(); else qCritical() << "Error" << mSerial->errorString();
        if(!mList.size()) cm::mdelay(msecs);
    }
}

QByteArray SerialPort::readSerial(int msecs)
{
    QByteArray rcv, array;
    if(mSerial->isReadable()) {        
        //mSerial->waitForReadyRead(msecs);
        for(int i=0; i<msecs; i+=10) {
            rcv = mSerial->readAll();
            if(rcv.size()) break; else cm::mdelay(10);
        }
        do{
            cm::mdelay(msecs/5);
            array = mSerial->readAll();
            rcv.append(array);
        } while (array.size());
    }

    return rcv;
}

QByteArray SerialPort::transmit(const QByteArray &array, int msecs)
{
    QByteArray rcv; if(mSerial->isOpen()) {
    QWriteLocker locker(mRwLock); mSerial->readAll();
    if(mSerial->write(array) > 0) {
        mSerial->flush(); rcv = readSerial(msecs);
    } else qCritical() << mSerial->errorString();}
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

void SerialPort::setBaudRate(qint32 br)
{
    mBr = br; QTimer::singleShot(50,this,SLOT(setBaudRateSlot()));
}
