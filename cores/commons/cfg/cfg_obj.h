#ifndef CFG_OBJ_H
#define CFG_OBJ_H
#include "file.h"

#define CFG_FN "cfg.ini"

class Cfg_Obj
{
    Cfg_Obj(const QString& fn, QObject *parent = nullptr);
public:
    static Cfg_Obj* bulid(const QString& fn, QObject *parent = nullptr);
    static QString pathOfCfg(const QString& name);

    void writeCfg(const QMap<QString,QVariant> &map, const QString &g="Cfg");
    void writeCfg(const QString &key, const QVariant& v, const QString &g="Cfg");

    QVariantList readCfg(const QStringList &keys, const QString &g="Cfg");
    QMap<QString,QVariant> readCfg(const QMap<QString, QVariant> &keys, const QString &g="Cfg");
    QVariant readCfg(const QString &key, const QVariant &v = QVariant(), const QString &g="Cfg");

private:
    static void initCfg();
    bool openCfg(QObject *parent = nullptr, const QString& fn = "cfg.ini");

private:
    static QSettings  *mCfgIni; //*mCfgIni;
};

#endif // CFG_OBJ_H
