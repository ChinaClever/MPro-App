#ifndef WEB_OBJ_H
#define WEB_OBJ_H
#include <QtCore>
#include "mongoose.h"
#include "jsonrpc_client.h"

class Web_Obj : public QObject
{
    explicit Web_Obj(QObject *parent = nullptr);
public:
    static Web_Obj *bulid(QObject *parent = nullptr);
    uint getNumber(mg_str &r, int id);
    QString getString(mg_str &r, int id);
    QVector<uint> getNumbers(mg_str &r, int num);

    QString metaData(uint addr);
    QString execute(const QString &cmd);
    QString getCfg(uint type, uint fc, uint id, uint addr);
    int setCfg(uint type, uint fc, const QVariant &value, uint id, uint addr);
    int setting(uint addr, uint type, uint topic, uint sub, uint id, double value);
    double getValue(uint addr, uint type, uint topic, uint sub, uint id);
    QVariant log_fun(uint type, uint fc, uint id, uint cnt);
    QString log_hda(const QString &start, const QString &end, int addr, int type, int topic, int index);

    bool app_upgrade(const QString &fn); // 升级文件是绝对路径
    bool restores(int fc, const QString &fn); // fc 1 配置文件 2 批量配置文件
    QString backup(int fc); // fc 1 配置文件 2 批量配置文件
    QString diag(); // 下载诊断功能
    QString getIpv6En(); // ipv6标志
    QString createUuid();
    bool checkUuid(mg_str &r);
    bool checkUuid(const QString &uuid);

private:
    JsonRpc_Client *mRpc;
    QString mUuid = " ";
};

#define cout qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "[" << __FILE__ << ":" << Q_FUNC_INFO << ":" << __LINE__ << "]"

#endif // WEB_OBJ_H
