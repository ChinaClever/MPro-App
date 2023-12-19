#ifndef WEB_CORE_H
#define WEB_CORE_H

#include "web_http.h"
#define SUPPORT_C 1
#include "/home/lzy/work/mpro-app/cores/commons/cms/datapacket.h"
#define SHM_KEY	0x5105


class Web_Core : public Web_Http
{
    Q_OBJECT
    explicit Web_Core(QObject *parent = nullptr);
public:
    static Web_Core *bulid(QObject *parent = nullptr);
    bool app_upgrade(const QString &fn);

private:
    void web_initFun(char *ip);
    sDataPacket *init_share_mem();
    void init_webCfg(sWebCfg &web);

private slots:
    void initFunSlot();
    void web_onTimeoutDone();

private:
    QTimer *timer = nullptr;
    sRunTime *mRun = nullptr;
};

#endif // WEB_CORE_H
