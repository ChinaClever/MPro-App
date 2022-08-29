#ifndef OP_ZRTU_H
#define OP_ZRTU_H

#include "op_zctrl.h"

class OP_ZRtu : public OP_ZCtrl
{
    Q_OBJECT
public:
    explicit OP_ZRtu(QObject *parent = nullptr);

public slots:
    void run();

private:
    bool readData(int addr) override;
    bool sendReadCmd(int addr, sOpIt *it);
    bool setEndisable(int addr, bool ret, uchar &v);
    bool recvPacket(const QByteArray &array, sOpIt *obj);
    void hardwareLog(int addr, const QByteArray &cmd);


protected:
    bool isRun=true;

private:
    QByteArray m_array[DEV_NUM];
};

#endif // OP_ZRTU_H
