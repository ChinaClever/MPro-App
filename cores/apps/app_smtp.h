#ifndef APP_SMTP_H
#define APP_SMTP_H

#include "app_ntp.h"
#define SMTP_TO_SIZE 4
struct sSmtpCfg {
    int en;
    QString host; //发件箱服务器
    int port;   // SMTP端口:
    int ct;  // 工作模式:

    QString from; // 发件箱账号:
    QString pwd; // 发件箱密码:
    QString to[SMTP_TO_SIZE]; // 收件用户账号:
    QString lastErr;
};

class App_Smtp : public App_Ntp
{
public:
    explicit App_Smtp(QObject *parent = nullptr);
    void smtp_sendMail(const QString &content);
    static sSmtpCfg smtpCfg;
    void smtp_run();

private:
    void sendMail();

private:
    QStringList mList;
    bool smtp_isRun=false;
};

#endif // APP_SMTP_H
