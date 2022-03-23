#ifndef FILE_H
#define FILE_H
#include <QtCore>

struct sFileTrans {
    ushort fc;
    QString recvPath;
    QString file;
    QString md5;
    ushort crc;
};


namespace File {
    QString Md5(const QString &fn);
    bool CheckMd5(const sFileTrans &it);
    bool CheckMd5(const QString &fn);
    bool AppendMd5(const QString &fn);
}


#endif // FILE_H
