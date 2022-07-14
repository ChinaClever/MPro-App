#ifndef MB_CORE_H
#define MB_CORE_H

#include "mb_update.h"

class Mb_Core : public QRunnable
{
    explicit Mb_Core(QObject *parent = nullptr);
public:
    static Mb_Core *bulid(QObject *parent = nullptr);
    void disconnectRtu() {mRtu->disconnectModbus();}
    void disconnectTcp() {mTcp->disconnectModbus();}
    void setRtu(int parameter, const QVariant &value);
    sModbusSetting *cfg() {return mCfg;}
    void setAddress(int addr);
    void setPort(int port);

public slots:
    void run() override;
    bool connectTcp(int en);
    bool connectRtu(int en);

private:
    void initFun();
    bool connectModbus(Mb_Update *mb, bool en, int rt);

private:
    bool isRun=true;
    Mb_Update *mRtu;
    Mb_Update *mTcp;
    sModbusSetting *mCfg;
};

#endif // MB_CORE_H
