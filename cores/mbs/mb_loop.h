#ifndef MB_LOOP_H
#define MB_LOOP_H

#include "mb_output.h"

class Mb_Loop : public Mb_Output
{
public:
    explicit Mb_Loop(QObject *parent = nullptr);

protected:
    void mbLoopUpdate();

private:
    void upLoopData();
    void upLoopThreshold();
};

#endif // MB_LOOP_H
