#ifndef OP_CORE_H
#define OP_CORE_H

#include "op_prortu.h"

class OP_Core : public OP_ProRtu
{
    explicit OP_Core(QObject *parent = nullptr);
public:
    static OP_Core *bulid(QObject *parent = nullptr);
    ~OP_Core(){isRun = false;}
    void run();

private:
    void initFunSlot();

private:
    bool isRun=true;
};

#endif // OP_CORE_H
