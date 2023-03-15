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

}

void App_Smtp::smtp_sendMail(const QString &content, bool ok)
{
    if(smtpCfg.en || ok) {
        mList << content;
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

void App_Smtp::sendMail()
{
    //if(!smtpCfg.en) return;
    sSmtpCfg *cfg = &smtpCfg;

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

    QString contents;
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
    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        cfg->lastErr += "Failed to connect to host!" + cfg->host;
    }

    smtp.login(cfg->from, cfg->pwd);
    if (!smtp.waitForAuthenticated()) {
        smtp.login(cfg->from, cfg->pwd, SmtpClient::AuthPlain);
        if (!smtp.waitForAuthenticated()) cfg->lastErr += " Failed to login!" + cfg->from;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        cfg->lastErr += " Failed to send mail!";
    }
    smtp.quit();
}

void App_Smtp::smtp_run()
{
    if(!smtp_isRun) {
        smtp_isRun = true;
        //cm::mdelay(500);
        sendMail();
        smtp_isRun = false;
    }
}
