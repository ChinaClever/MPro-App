#ifndef APP_SSH_H
#define APP_SSH_H

#include "app_smtp.h"

struct sSshCfg {
    sSshCfg(){ssh_en=1; telnet_en=0;}
    int ssh_en;
    int telnet_en;
    QString usr;
    QString pwd;
};

class App_Ssh : public App_Smtp
{
    Q_OBJECT
public:
    explicit App_Ssh(QObject *parent = nullptr);
    static sSshCfg sshCfg;
    bool ssh_save();

private:
    void ssh_delUser();
    bool ssh_addrUser();

private slots:
    void ssh_initFunSlot();
};

#endif // APP_SSH_H
