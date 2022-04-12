#ifndef OP_ZRTU_H
#define OP_ZRTU_H

#include "op_zctrl.h"

class OP_ZRtu : public OP_ZCtrl
{
    Q_OBJECT
    explicit OP_ZRtu(QObject *parent = nullptr);
public:
    static OP_ZRtu *bulid(QObject *parent = nullptr);
    ~OP_ZRtu();

public slots:
    void run();
    void start();

private:
    bool readData(int addr) override;
    bool sendReadCmd(int addr, sOpIt *it);
    bool recvPacket(const QByteArray &array, sOpIt *obj);

private:
    bool isRun;
    CThread *mThread;
};

using OP_Core = OP_ZRtu;
#endif // OP_ZRTU_H
