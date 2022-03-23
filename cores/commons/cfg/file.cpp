#include "file.h"

QString File::Md5(const QString &fn)
{
    QFile sourceFile(fn);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 1024;

    if (sourceFile.open(QIODevice::ReadOnly)) {
        char buffer[bufferSize]; int bytesRead;
        int readSize = qMin(fileSize, bufferSize);
        QCryptographicHash hash(QCryptographicHash::Md5);
        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0) {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }
        sourceFile.close();
        return QString(hash.result().toHex());
    }

    return QString();
}

bool File::CheckMd5(const sFileTrans &it)
{
    QString crc = Md5(it.recvPath + it.file);
    return it.crc == crc;
}

bool File::CheckMd5(const QString &fn)
{
     QString ret = fn.split(".").last();
     QString crc = Md5(fn);
     return ret == crc;
}

bool File::AppendMd5(const QString &fn)
{
    QString crc = Md5(fn);
    return QFile::rename(fn, fn+"."+crc);
}
