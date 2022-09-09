#ifndef CFG_OBJ_H
#define CFG_OBJ_H
#include "file.h"

#define CFG_FN "cfg.ini"

class Cfg_Obj
{
public:
    Cfg_Obj(const QString& fn, QObject *parent = nullptr);
    static Cfg_Obj* bulid(const QString& fn=CFG_FN, QObject *parent = nullptr);
    static QString pathOfCfg(const QString& name);

    void writeCfg(const QMap<QString,QVariant> &map, const QString &g="");
    void writeCfg(const QString &key, const QVariant& v, const QString &g="");

    QVariantList readCfg(const QStringList &keys, const QString &g="");
    QMap<QString,QVariant> readCfg(const QMap<QString, QVariant> &keys, const QString &g="");
    QVariant readCfg(const QString &key, const QVariant &v = QVariant(), const QString &g="");

private:
    static void initCfg();
    bool openCfg(QObject *parent = nullptr, const QString& fn = CFG_FN);

private:
    QSettings *mCfgIni=nullptr;
};

#endif // CFG_OBJ_H
