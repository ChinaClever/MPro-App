#ifndef MB_CORE_H
#define MB_CORE_H

#include "mb_restore.h"
#define MB_NAME  "/dev/ttyUSB1"

class Mb_Core : public QRunnable
{
    explicit Mb_Core(QObject *parent = nullptr);
public:
    static Mb_Core *bulid(QObject *parent = nullptr);
    bool connectRtu(int addr=1, int baud = QSerialPort::Baud9600, const QString &name=MB_NAME);
    void disconnectRtu() {mRtu->disconnectModbus();}

    bool connectTcp(int port=1502);

//protected:
    void run();

private:
    bool isRun=true;
    Mb_Restore *mRtu;
    Mb_Restore *mTcp;
};

#endif // MB_CORE_H
