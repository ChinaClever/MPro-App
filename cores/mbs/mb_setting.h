#ifndef MB_SETTING_H
#define MB_SETTING_H

#include "mb_env.h"

class Mb_Setting : public Mb_Env
{
    Q_OBJECT
public:
    explicit Mb_Setting(QObject *parent = nullptr);

protected:
    void mbSetUpdate();

private:
    void upSetData();
};

#endif // MB_SETTING_H
