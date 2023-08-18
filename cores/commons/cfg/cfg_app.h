#ifndef CFG_APP_H
#define CFG_APP_H
#include "cfg_com.h"

#define CFG_APP  "ver.ini"

struct sAppVerIt {
    QString sn; // 序列号
    QString hw; // 硬件版本
    QString usr; // 客户名称
    QString md5; // 校验码
    QString sig; // 签名
    QString ver; // 版本号
    QString dev; // 设备类型
    QString remark; // 发布说明
    QStringList apps; // 程序名称
    QString oldVersion; // 依赖版本号
    QString releaseDate; // 发布时间
    QString upgradeDate; // 升级日期
};


class Cfg_App : public Cfg_Obj
{
public:
    Cfg_App(const QString& dir, QObject *parent = nullptr);
    void app_serialNumber(const QString &sn);
    void app_oldVersion(const QString &ver);
    bool app_check(const QString &fn);
    bool app_pack(sAppVerIt &it);
    bool app_unpack(sAppVerIt &it);
    void app_upgradeDate();

private:
    void app_dir(const QString &dir);

private:
    QString mDir;
};

#endif // CFG_APP_H
