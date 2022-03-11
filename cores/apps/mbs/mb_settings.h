#ifndef MB_SETTINGS_H
#define MB_SETTINGS_H

#include "mb_outputs.h"

class Mb_Settings : public Mb_Outputs
{
    Q_OBJECT
public:
    explicit Mb_Settings(QObject *parent = nullptr);

signals:

};

#endif // MB_SETTINGS_H
