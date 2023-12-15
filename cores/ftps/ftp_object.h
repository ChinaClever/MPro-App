#ifndef FTP_OBJECT_H
#define FTP_OBJECT_H
#include "commons.h"

struct sFtpCfg
{
    int en = 4; // 0禁用  1 ftp  2 scp 3 FTPS 4 SFTP
    QString host = "192.168.1.102"; // 服务端地址
    QString user = "lzy"; // 账号
    QString pwd = "123456";  // 地址
    QString path = "/"; // 路径
    int port = 0;
    int total = 1; //
    int line = 1;
    int loop = 1;
    int outlet = 1;
    int dualPower = 1;
    int updateTime = 1;
    int backupTime = 1;
    int csvXlsx = 0;
};

class Ftp_Object
{
public:
    Ftp_Object();
    static sFtpCfg ftpCfg;
    QString mDir="/tmp/csv/";
    enum {Disable, Ftp, Scp, Ftps, Sftp};
};

#endif // FTP_OBJECT_H
