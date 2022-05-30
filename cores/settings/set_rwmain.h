#ifndef SET_RWMAIN_H
#define SET_RWMAIN_H

#include "set_rwparam.h"

class Set_RwMain : public Set_RwParam
{
    Q_OBJECT
    explicit Set_RwMain(QObject *parent = nullptr);
public:
    static Set_RwMain *bulid(QObject *parent = nullptr);
    bool readDefault() {return readSetting(SET_DEFAULT_FN);}
    void writeDefault();

private slots:
    bool readSettings();

};

using Set_ReadWrite = Set_RwMain;
#endif // SET_RWMAIN_H
