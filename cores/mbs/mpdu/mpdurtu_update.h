#ifndef MPDURTU_UPDATE_H
#define MPDURTU_UPDATE_H

#include "mpdurtu_control.h"

class Mpdurtu_Update : public Mpdurtu_Control
{
public:
    explicit Mpdurtu_Update(QObject *parent = nullptr);
    void mbUpdates(int addr);
};

#endif // MPDURTU_UPDATE_H
