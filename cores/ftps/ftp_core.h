#ifndef FTP_CORE_H
#define FTP_CORE_H
#include "ftp_data.h"

class Ftp_Core : public Ftp_Data
{
    Ftp_Core();
public:
    static Ftp_Core *bulid();
    ~Ftp_Core();
    void run();

private:
    void workDown();
    void mdelay() {cm::mdelay(60*1000); mCnt++;}

private:
    int mCnt = 0;
    bool isRun = true;
};

#endif // FTP_CORE_H
