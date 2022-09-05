#ifndef APP_SMTP_H
#define APP_SMTP_H

#include "app_ntp.h"

struct sSmtpCfg {
    int en;
    QString host; //发件箱服务器
    int port;   // SMTP端口:
    int ct;  // 工作模式:

    QString from; // 发件箱账号:
    QString pwd; // 发件箱密码:
    QString to; // 收件用户账号:
    QString lastErr;
};

class App_Smtp : public App_Ntp
{
    Q_OBJECT
public:
    explicit App_Smtp(QObject *parent = nullptr);
    void smtp_sendMail(const QString &subject, const QString &content);
    static sSmtpCfg smtpCfg;

signals:
    void sendMailSig(const QString &subject, const QString &content);

private slots:
    void sendMailSlot(const QString &subject, const QString &content);

};

#endif // APP_SMTP_H