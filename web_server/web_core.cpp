/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_core.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#define setbit(x,y) x|=(1<<y)
static QStringList g_ip_addr;
QStringList web_ip_addr(){return g_ip_addr;}

Web_Core::Web_Core(QObject *parent) : Web_Http{parent}
{
    QTimer::singleShot(4634,this,SLOT(initFunSlot()));
    QTimer::singleShot(7840,this,SLOT(netAddrSlot()));
}

Web_Core *Web_Core::bulid(QObject *parent)
{
    static Web_Core* sington = nullptr;
    if(sington == nullptr) sington = new Web_Core(parent);
    return sington;
}

void Web_Core::netAddrSlot()
{
    g_ip_addr.clear(); for(int i=0; i<3; ++i) {
        QString ptr = m_shm->net.inet6.reserve[i];
        if(ptr.size() > 3) g_ip_addr << QString("[%1]").arg(ptr.split("/").first());
    } g_ip_addr << QString("[%1]").arg(m_shm->net.inet6.ip);
    g_ip_addr << m_shm->net.inet.ip; g_ip_addr.removeDuplicates();
    if(m_shm->net.inet.dhcp || m_shm->net.inet6.dhcp) Web_Obj::bulid()->clearUuid();
}

void Web_Core::initFunSlot()
{
    sDataPacket *shm = m_shm = init_share_mem();
    timer = new QTimer(this); timer->start(1000); mgr_init();
    connect(timer, SIGNAL(timeout()),this, SLOT(web_onTimeoutDone()));
    if(mRun) {
        web_initFun(); int t = 150;
        if(shm->net.inet.dhcp || shm->net.inet6.dhcp) t += 2840;
        QTimer::singleShot(t,this,SLOT(netAddrSlot()));
    }
}

void Web_Core::init_webCfg(sWebCfg &web)
{
    cfg.http_en = web.http_en;
    cfg.http_port = web.http_port;
    cfg.http_redirect = web.http_redirect;

    cfg.https_en = web.https_en;
    cfg.https_port = web.https_port;
    cfg.multi_users = web.multi_users;
}

sDataPacket *Web_Core::init_share_mem()
{
    static struct sDataPacket *shm = NULL; if(shm) return shm;
    key_t key = ftok("/tmp", SHM_KEY);
    int shmid = shmget(key, sizeof(struct sDataPacket), 0666|IPC_CREAT);    //创建共享内存
    if(shmid >= 0) {
        shm = (sDataPacket *)shmat(shmid, 0, 0);  //将共享内存连接到当前进程的地址空间
        if(shm) {mRun = &(shm->data->proc.web); init_webCfg(shm->web);}
    } else fprintf(stderr, "shmget failed %d\n", shmid);
    return shm;
}

void Web_Core::web_initFun()
{
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(mRun->start, t.toLatin1().data());

    QString dateTime;
    dateTime += __DATE__; dateTime += __TIME__;
    dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
    QDateTime dt = QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
    QString str = dt.toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(mRun->compileTime, str.toUtf8().data());
}

void Web_Core::web_onTimeoutDone()
{
    if(mRun)mRun->runSec += 1;
}

bool Web_Core::app_upgrade(const QString &fn)
{
    sDataPacket *shm = init_share_mem(); qDebug() <<fn;
    bool ret = true; sOtaUpdater *ota = &(shm->ota);
    if(fn.contains("/usr/data/upload/")) {
        setbit(ota->work, DOta_Web);
    } else  ret = false;    
    return ret;
}
