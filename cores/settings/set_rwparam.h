#ifndef SET_RWPARAM_H
#define SET_RWPARAM_H

#include "set_rwinitial.h"

class Set_RwParam : public Set_RwInitial
{
public:
    explicit Set_RwParam(QObject *parent = nullptr);
    void readCfgParams();

private:
    void readUut();
    void outputName();

private:
     Cfg_Obj *mCfg;
};

#endif // SET_RWPARAM_H
