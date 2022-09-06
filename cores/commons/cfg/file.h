#ifndef FILE_H
#define FILE_H
#include <QtCore>
#include <QMetaType>

struct sFileTrans {
    ushort fc;
    QString dev;
    QString path;
    QString file;
    QString md5;
    uint size;
    ushort crc;
};


namespace File {
    QString Md5(const QString &fn);
    bool CheckMd5(const sFileTrans &it);
    bool CheckMd5(const QString &fn);
    bool AppendMd5(const QString &fn);

    bool CheckCrc(const QString &fn);
    bool AppendCrc(const QString &fn);
    uint Size(const QString &fn);

    QStringList entryList(const QString &p="");

    QString certFile();
    QString keyFile();
}


#endif // FILE_H
