#ifndef MB_RESTORE_H
#define MB_RESTORE_H

#include "mb_control.h"

class Mb_Restore : public Mb_Control
{
    Q_OBJECT
public:
    explicit Mb_Restore(QObject *parent = nullptr);
    void mbUpdates();

};

#endif // MB_RESTORE_H
