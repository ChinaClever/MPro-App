#ifndef MB_SETTING_H
#define MB_SETTING_H

#include "mb_output.h"

class Mb_Setting : public Mb_Output
{
    Q_OBJECT
public:
    explicit Mb_Setting(QObject *parent = nullptr);

signals:

};

#endif // MB_SETTING_H
