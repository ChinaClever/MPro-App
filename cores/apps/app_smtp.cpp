/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_smtp.h"
#include "smtp/SmtpMime"

sSmtpCfg App_Smtp::smtpCfg;
App_Smtp::App_Smtp(QObject *parent)
    : App_Ntp{parent}
{
    connect(this, &App_Smtp::sendMailSig, this, &App_Smtp::sendMailSlot);
}

void App_Smtp::smtp_sendMail(const QString &subject, const QString &content)
{
    emit sendMailSig(subject, content);
}

void App_Smtp::sendMailSlot(const QString &subject, const QString &content)
{
    if(!smtpCfg.en) return;
    sSmtpCfg *cfg = &smtpCfg;

    MimeMessage message;
    EmailAddress sender(cfg->from);
    message.setSender(sender);

    EmailAddress to(cfg->to);
    message.addRecipient(to);
    message.setSubject(subject);

    MimeText text;
    text.setText(content);
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
        cfg->lastErr = "Failed to connect to host!";
    }

    smtp.login(cfg->from, cfg->pwd);
    if (!smtp.waitForAuthenticated()) {
        cfg->lastErr = "Failed to login!";
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        cfg->lastErr = "Failed to send mail!";
    }

    cfg->lastErr.clear();
    smtp.quit();
}
