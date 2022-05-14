#ifndef OP_CORE_H
#define OP_CORE_H

#include "op_zrtu.h"

class OP_Core : public OP_ZRtu
{
    Q_OBJECT
    explicit OP_Core(QObject *parent = nullptr);
public:
    static OP_Core *bulid(QObject *parent = nullptr);
    ~OP_Core(){isRun = false;}
    void startFun();

private:
    CThread *mThread;
};

#endif // OP_CORE_H
