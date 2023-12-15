#ifndef MB_UPDATE_H
#define MB_UPDATE_H

#include "mb_control.h"

class Mb_Update : public Mb_Control
{
public:
    explicit Mb_Update(QObject *parent = nullptr);
    void mbUpdates(int addr);
};

#endif // MB_UPDATE_H
