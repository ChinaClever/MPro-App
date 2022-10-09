#ifndef MB_CORE_H
#define MB_CORE_H

#include "mb_update.h"

class Mb_Core : public QThread
{
    Q_OBJECT
    explicit Mb_Core(QObject *parent = nullptr);
public:
    static Mb_Core *bulid(QObject *parent = nullptr);
    void disconnectRtu() {mRtu->disconnectModbus();}
    void disconnectTcp() {mTcp->disconnectModbus();}
    void setRtu(int parameter, const QVariant &value);
    sModbusSetting *cfg() {return mCfg;}
    static sModbusSetting modbusCfg;
    void setAddress(int addr);
    void setPort(int port);

signals:
    void connectTcpSig();
    void connectRtuSig();

public slots:
    void run();
    void initFunSlot();
    void connectTcpSlot();
    void connectRtuSlot();


private:
    bool isRun=true;
    Mb_Update *mRtu;
    Mb_Update *mTcp;
    CThread *mThread;
    sModbusSetting *mCfg;
};

#endif // MB_CORE_H
