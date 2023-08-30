/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_upload.h"

Ftp_Upload::Ftp_Upload()
{

}

void Ftp_Upload::uploads(const QString &fn)
{
    switch (ftpCfg.en) {
    case Sftp: upload_sftp(fn); break;
    case Ftps: upload_ftps(fn); break;
    case Scp: upload_scp(fn); break;
    case Ftp: upload_ftp(fn); break;
    default: cout << fn; break;
    }
}

void Ftp_Upload::upload_ftp(const QString &fn)
{
    QString fmd = "curl -u %1:%2 -T %3 ftp://%4:%5 &"; sFtpCfg *cfg = &ftpCfg;
    QString cmd = fmd.arg(cfg->user, cfg->pwd, mDir+fn, cfg->host, cfg->path);
    upload_system(cmd);
}

void Ftp_Upload::upload_ftps(const QString &fn)
{
    QString fmd = "curl -k -u %1:%2 -T %3 ftps://%4:%5 &"; sFtpCfg *cfg = &ftpCfg;
    QString cmd = fmd.arg(cfg->user, cfg->pwd, mDir+fn, cfg->host, cfg->path);
    upload_system(cmd);
}

void Ftp_Upload::upload_scp(const QString &fn)
{
    QString fmd = "yes '%1' | sshpass -p '%1' scp -o StrictHostKeyChecking=no '%2' '%3@%4:%5' &";
    sFtpCfg *cfg = &ftpCfg; QString cmd = fmd.arg(cfg->pwd, mDir+fn, cfg->user, cfg->host, cfg->path);
    system("killall yes"); upload_system(cmd);
}

void Ftp_Upload::upload_sftp(const QString &fn)
{
    QString fmd = "sshpass -p '%1' sftp -o StrictHostKeyChecking=no '%2@%3' <<EOF \n put '%4' '%5' \n exit \n EOF \n";
    sFtpCfg *cfg = &ftpCfg; QString cmd = fmd.arg(cfg->pwd, cfg->user, cfg->host, mDir+fn, cfg->path);
    upload_system(cmd);
}

void Ftp_Upload::upload_system(const QString &cmd)
{
    int t = 540 + QRandomGenerator::global()->bounded(800);
    cm::mdelay(t); system(cmd.toStdString().c_str());
}
