#ifndef CFG_FILE_H
#define CFG_FILE_H
#include "cfg_obj.h"
#include <QMetaType>

struct sOtaFile {
    ushort fc;
    QString dev;
    QString path;
    QString file;
    QString md5;
    uint size;
    ushort crc;
};

namespace File {
    QString md5(const QString &fn);
    bool CheckMd5(const sOtaFile &it);
    bool CheckMd5(const QString &fn);
    bool AppendMd5(const QString &fn);

#ifdef CRC_H
    bool CheckCrc(const QString &fn);
    bool AppendCrc(const QString &fn);
#endif

    uint size(const QString &fn);
    QStringList entryList(const QString &p="");

    QString certFile();
    QString keyFile();
}


#endif // CFG_FILE_H
