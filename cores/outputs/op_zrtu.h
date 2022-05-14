#ifndef OP_ZRTU_H
#define OP_ZRTU_H

#include "op_zctrl.h"

class OP_ZRtu : public OP_ZCtrl //, public QRunnable
{
    Q_OBJECT
    explicit OP_ZRtu(QObject *parent = nullptr);
public:
    static OP_ZRtu *bulid(QObject *parent = nullptr);
    ~OP_ZRtu(){isRun = false;}
    void startFun();

public slots:
    void run();// override;

private:
    bool readData(int addr) override;
    bool sendReadCmd(int addr, sOpIt *it);
    bool setEndisable(int addr, bool ret, uchar &v);
    bool recvPacket(const QByteArray &array, sOpIt *obj);

private:
    bool isRun=true;
    QTimer *timer;
    CThread *mThread;
};

using OP_Core = OP_ZRtu;
#endif // OP_ZRTU_H
