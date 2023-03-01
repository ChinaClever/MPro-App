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
    mSerial = new Serial_Port();
    mRwLock = new QReadWriteLock;
}

bool SerialPort::openSerial(const QString &name,qint32 baudRate, QSerialPort::Parity parity)
{
    mSerial->closePort(); mSerial->setPortName(name);
    bool ret = mSerial->openPort(); // QIODevice::ReadWrite
    if(ret) { mBr = baudRate;
        // mSerial->setParity(parity);    //无奇偶校验
        // mSerial->setBaudRate(baudRate);  //波特率
        // mSerial->setDataBits(QSerialPort::Data8); //数据位
        // mSerial->setStopBits(QSerialPort::OneStop);   //无停止位
        // mSerial->setFlowControl(QSerialPort::NoFlowControl);  //无控制

        int p = 'N';
        switch (parity) {
        case QSerialPort::EvenParity: p = 'E'; break;
        case QSerialPort::OddParity: p = 'O'; break;
        default: p = 'N'; break;
        } ret = mSerial->setup(baudRate, 8, 1, p);
        if(ret) qDebug() << "serial setup 0k" << name << baudRate;
        else cout << "serial setup err" << name << baudRate;
    } else cout << name;
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
    if(ret) mCmdList << array;
    return ret;
}

void SerialPort::waitForSend(int size)
{
    if((size > 0) && (mBr > 0)) {
        int ms = (size*10.0)/mBr*1000;
        cm::mdelay(ms+50);
    }
}

void SerialPort::cmsWrite(int msecs)
{
    QWriteLocker locker(mRwLock);  while(mCmdList.size()) {
        cm::mdelay(msecs); int ret = mSerial->writePort(mCmdList.takeFirst());
        if(ret > 0) mSerial->flush(); else cout << "Error" << ret;  //mSerial->errorString();
        waitForSend(ret); cm::mdelay(msecs);
    }
}

QByteArray SerialPort::readSerial(int msecs)
{
    QByteArray rcv, array;
    if(mSerial->isReadable()) {
        //mSerial->waitForReadyRead(msecs);
        for(int i=0; i<msecs; i+=50) {
            rcv = mSerial->readAll();
            if(rcv.size()) break;
            else cm::mdelay(50);
        }

        do{
            cm::mdelay(75);
            array = mSerial->readAll();
            if(array.isEmpty()) {
                cm::mdelay(85);
                array += mSerial->readAll();
            } rcv.append(array);
        } while (array.size());
    }

    return rcv;
}

QByteArray SerialPort::transmit(const QByteArray &array, int msecs)
{
    QByteArray rcv; if(mSerial->isOpen()) {
        QWriteLocker locker(mRwLock); mSerial->readAll();
        if(mSerial->writePort(array) > 0) {
            mSerial->flush();
            waitForSend(array.size());
            rcv = readSerial(msecs);
        } else cout << array; //mSerial->errorString();
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

