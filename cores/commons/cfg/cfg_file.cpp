/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_file.h"
#include "crc.h"

QStringList File::entryList(const QString &p)
{
    QString path = QDir::currentPath();//获取当前工程目录
    if(p.size()) path = p;
    QDir dir(path); QStringList filename;
    //filename << "*.png" << "*.jpg";//可叠加，可使用通配符筛选
    QStringList results;
    results = dir.entryList(filename, QDir::Files|QDir::Readable, QDir::Name);
    return results;
}

QString File::md5(const QString &fn)
{
    QFile sourceFile(fn);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 8*1024;

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
    QString crc = md5(it.path + it.file);
    return it.md5 == crc;
}

bool File::CheckMd5(const QString &fn)
{
    QString ret = fn.split(".").last();
    QString crc = md5(fn);
    return ret == crc;
}

bool File::AppendMd5(const QString &fn)
{
    QString crc = md5(fn);
    return QFile::rename(fn, fn+"."+crc);
}

bool File::CheckCrc(const QString &fn)
{
    uint crc = Crc::File(fn);
    QString ret = fn.split(".").last();
    QString str = QByteArray::number(crc).toHex();
    return ret==str;
}

bool File::AppendCrc(const QString &fn)
{
    uint crc = Crc::File(fn);
    QByteArray b = QByteArray::number(crc).toHex();
    return QFile::rename(fn, fn+"."+b);
}

uint File::size(const QString &fn)
{
    uint size = 0; QFile file(fn);
    if (file.open(QIODevice::ReadOnly)) {
        size  = file.size();
        file.close();
    }
    return size;
}

QString File::keyFile()
{
    QString fn;
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    fn = "certs/key.pem";
#else
    QString key = "/usr/data/clever/certs/client-key.pem";
    QString cert = "/usr/data/clever/certs/client-cert.pem";
    if(QFile::exists(key) && QFile::exists(cert)) fn = key;
    else fn = "/etc/ssl/certs/key.pem";
#endif
    return fn;
}

QString File::certFile()
{
    QString fn;
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    fn = "certs/cert.pem";
#else
    QString key = "/usr/data/clever/certs/client-key.pem";
    QString cert = "/usr/data/clever/certs/client-cert.pem";
    if(QFile::exists(key) && QFile::exists(cert)) fn = cert;
    else fn = "/etc/ssl/certs/cert.pem";
#endif
    return fn;
}
