#ifndef WEB_CORE_H
#define WEB_CORE_H

#include "web_http.h"
#define SUPPORT_C 1
#include "../cores/commons/cms/datapacket.h"
#define SHM_KEY	0x5105


class Web_Core : public Web_Http
{
    Q_OBJECT
    explicit Web_Core(QObject *parent = nullptr);
public:
    static Web_Core *bulid(QObject *parent = nullptr);
    bool app_upgrade(const QString &fn);

private:
    void web_initFun();    
    sDataPacket *init_share_mem();
    void init_webCfg(sWebCfg &web);

private slots:
    void initFunSlot();
    void netAddrSlot();
    void web_onTimeoutDone();

private:
    QTimer *timer = nullptr;
    sRunTime *mRun = nullptr;
    sDataPacket *m_shm = nullptr;
};

#endif // WEB_CORE_H
