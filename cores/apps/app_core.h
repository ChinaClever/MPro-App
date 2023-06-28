#ifndef APP_CORE_H
#define APP_CORE_H

#include "app_whitelist.h"

class App_Core : public App_WhiteList
{
    explicit App_Core(QObject *parent = nullptr);
public:
    static App_Core *bulid(QObject *parent = nullptr);

private:
    void initVer();
    void initUuid();
    void compileTime();
    void initRoot(const QString &sn);
    QString hashPassword(const QString& password);
};

#endif // APP_CORE_H
