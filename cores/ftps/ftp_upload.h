#ifndef FTP_UPLOAD_H
#define FTP_UPLOAD_H
#include "ftp_object.h"

class Ftp_Upload : public Ftp_Object
{
public:
    Ftp_Upload();
    void uploads(const QString &fn);

private:
    void upload_scp(const QString &fn);
    void upload_ftp(const QString &fn);
    void upload_ftps(const QString &fn);
    void upload_sftp(const QString &fn);
};

#endif // FTP_UPLOAD_H
