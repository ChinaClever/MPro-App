/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_core.h"

Ftp_Core::Ftp_Core()
{
    QtConcurrent::run(this,&Ftp_Core::run);
}

Ftp_Core *Ftp_Core::bulid()
{
    static Ftp_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Ftp_Core();
    }
    return sington;
}

void Ftp_Core::workDown()
{
    if(!ftpCfg.en) return ;
    if(!ftpCfg.updateTime || !ftpCfg.backupTime) return ;
    if(ftpCfg.user.isEmpty() || ftpCfg.pwd.isEmpty()) return ;
    if(ftpCfg.host.isEmpty() || ftpCfg.path.isEmpty()) return ;
    if(0 == (mCnt % ftpCfg.updateTime)) upDevData();
    if(0 == (mCnt % (ftpCfg.backupTime*60))) csv_saves();
}


void Ftp_Core::run()
{
    while(1) {
        mdelay();
        workDown();
    }
}
