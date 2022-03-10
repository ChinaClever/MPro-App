#ifndef OP_ZCTRL_H
#define OP_ZCTRL_H

#include "op_objctrl.h"

class OP_ZCtrl : public OP_ObjCtrl
{
    Q_OBJECT
public:
    explicit OP_ZCtrl(QObject *parent = nullptr);
    enum {zCmdLen=68};

private:
    void writeCtrlCmd(uchar *cmd, int k);
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
    void funDelay(uchar *buf);
};

#endif // OP_ZCTRL_H
