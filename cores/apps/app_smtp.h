#ifndef APP_SMTP_H
#define APP_SMTP_H

#include "app_radius.h"
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

class App_Smtp : public App_Radius
{
    Q_OBJECT
public:
    explicit App_Smtp(QObject *parent = nullptr);
    void smtp_sendMail(const QString &content, bool ok=false);
    static sSmtpCfg smtpCfg;
    bool smtp_testMail();

private slots:
    void smtp_run();
    void sendMail();

private:
    QStringList mList;
    bool smtp_isRun=false;
};

#endif // APP_SMTP_H
