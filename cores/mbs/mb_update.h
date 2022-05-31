#ifndef MB_UPDATE_H
#define MB_UPDATE_H

#include "mb_control.h"

class Mb_Update : public Mb_Control
{
    Q_OBJECT
public:
    explicit Mb_Update(QObject *parent = nullptr);
    void mbUpdates();
};

#endif // MB_UPDATE_H
