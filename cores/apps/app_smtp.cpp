/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_smtp.h"
#include "smtp/SmtpMime"

sSmtpCfg App_Smtp::smtpCfg;
App_Smtp::App_Smtp(QObject *parent)
    : App_Radius{parent}
{
    //QTimer::singleShot(5432,this,&App_Smtp::smtp_testMail);
}
/**
 * 发送SMTP邮件
 */
void App_Smtp::smtp_sendMail(const QString &content, bool ok)
{
    if(smtpCfg.en || ok) {     /*smtp配置启用*/
        mList << content;      /*将参数content添加到mList中*/
         if(!smtp_isRun) QTimer::singleShot(1345,this,&App_Smtp::smtp_run);
        //if(!smtp_isRun) QtConcurrent::run(this, &App_Smtp::smtp_run);
    }
}

bool App_Smtp::smtp_testMail()
{
    QString msg = "this is test mail";
    smtp_sendMail(msg, true);
    return true;
}

QString App_Smtp::smtp_uut()
{
    QString str;
    str = "ip addr:";
    str += cm::dataPacket()->net.inet.ip;
    str += "\r\n";
    return str;
}

void App_Smtp::smtp_testInit()
{
    sSmtpCfg *cfg = &smtpCfg;
    cfg->port = 25;
    cfg->host = "smtp.qq.com";
    cfg->pwd = "hltwgrkymjcbbjcd";
    //   cfg->pwd = "Lzy123456";
    cfg->from ="luozhiyong131@qq.com";
    cfg->ct = 0; cfg->en = 1;
    cfg->to[0] = "517345026@qq.com";

    //    cfg->port = 465;
    //    cfg->ct = 1;
}

void App_Smtp::sendMail()
{
    //if(!smtpCfg.en) return;
    sSmtpCfg *cfg = &smtpCfg;
    //smtp_testInit();

    MimeMessage message;
    EmailAddress sender(cfg->from);
    message.setSender(sender);
    cfg->lastErr.clear();

    for(int i=0; i<SMTP_TO_SIZE; ++i) {
        if(cfg->to[i].size()) {
            EmailAddress to(cfg->to[i]);
            message.addRecipient(to);
        }
    } message.setSubject("PDU Email");

    QString contents = smtp_uut();
    foreach(const auto &it, mList)
        contents += it + "\r\n";
    mList.clear();

    MimeText text;
    text.setText(contents);
    message.addPart(&text);

    SmtpClient::ConnectionType ct;
    switch (cfg->ct) {
    case 0: ct = SmtpClient::TcpConnection; break;
    case 1: ct = SmtpClient::SslConnection; break;
    case 2: ct = SmtpClient::TlsConnection; break;
    default: ct = SmtpClient::TcpConnection; break;
    }

    SmtpClient smtp(cfg->host, cfg->port, ct);
    smtp.connectToHost(); //cout <<"AAAAAAAAAAAA";
    if (!smtp.waitForReadyConnected(5000)) {
        cfg->lastErr += "Failed to connect to host! " + cfg->host;
        smtp.quit(); cout << cfg->lastErr; return;
    }

    smtp.login(cfg->from, cfg->pwd);
    //cout <<"BBBBBBBBBBBBBB";
    if (!smtp.waitForAuthenticated(5000)) {
        cfg->lastErr += " Failed to login!" + cfg->from;
        smtp.quit(); cout << cfg->lastErr; return;
    }

    //cout <<"CCCCCCCCCCCCCCCCCC";
    smtp.sendMail(message);
    if (!smtp.waitForMailSent(5000)) {
        cfg->lastErr += " Failed to send mail!";
    }
    cfg->lastErr = "send mail ok";
    smtp.quit(); cout << cfg->lastErr;
}

bool App_Smtp::smtp_inputCheck()
{
    QString s = ".com";
    sSmtpCfg *cfg = &smtpCfg;
    if(!cfg->host.contains(s)) {
        cfg->lastErr += "host error " + cfg->host;
        return false;
    }

    if(!cfg->from.contains(s)) {
        cfg->lastErr += "outbox error " + cfg->from;
        return false;
    }

    if(cfg->pwd.isEmpty()) {
        cfg->lastErr += "Password error " + cfg->pwd;
        return false;
    }

    bool ret = false;
    for(int i=0; i<SMTP_TO_SIZE; i++) {
        QString str = cfg->to[i];
        if(str.size() && !str.contains(s)) {
            cfg->lastErr += "inbox error " + str;
            return false;
        } else ret = true;
    }
    return ret;
}

void App_Smtp::smtp_run()
{
    if(!smtp_isRun) {
        smtp_isRun = true;        
        sSmtpCfg *cfg = &smtpCfg;
        cfg->lastErr.clear();
        bool ret = smtp_inputCheck();
        if(ret) { ret = cm::pingNet(cfg->host);
        if(ret) sendMail(); else cfg->lastErr += "network error or host error!";
        }  smtp_isRun = false;
    }
}
