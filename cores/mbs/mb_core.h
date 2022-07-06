#ifndef MB_CORE_H
#define MB_CORE_H

#include "mb_update.h"
#define MB_NAME  "/dev/ttyUSB0"

class Mb_Core : public QRunnable
{
    explicit Mb_Core(QObject *parent = nullptr);
public:
    static Mb_Core *bulid(QObject *parent = nullptr);
    bool connectRtu(int addr=1, int baud = QSerialPort::Baud57600, int parity=QSerialPort::EvenParity);
    void disconnectRtu() {mRtu->disconnectModbus();}
    void disconnectTcp() {mTcp->disconnectModbus();}
    bool connectTcp(int addr, int port=1502);
    void setAddress(int addr);

public slots:
    void run() override;

private:
    bool isRun=true;
    Mb_Update *mRtu;
    Mb_Update *mTcp;
};

#endif // MB_CORE_H
