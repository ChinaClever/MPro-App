#ifndef CFG_COM_H
#define CFG_COM_H
#include "cfg_file.h"

#define CFG_FN "cfg.ini"

class Cfg_Com  : public Cfg_Obj
{
    Cfg_Com(const QString& fn, QObject *parent = nullptr);
public:
    static Cfg_Com* bulid(QObject *parent = nullptr, const QString& fn=CFG_FN);

private:
    static void initCfg();
};

#endif // CFG_COM_H
