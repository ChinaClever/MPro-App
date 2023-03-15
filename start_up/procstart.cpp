/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "procstart.h"

ProcStart::ProcStart()
{

}

void ProcStart::proc_time(sRunTime &proc)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString str = dateTime.toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(proc.start, str.toUtf8().data());
}

void ProcStart::proc_md5(sRunTime &proc, const QString &fn)
{
    QString str = md5(fn); proc.resetCnt += 1;
    qstrcpy(proc.md5, str.toUtf8().data());
}

void ProcStart::proc_start(sRunTime &proc, const QString &app)
{
    QString path = ""; //"/usr/data/clever/app/";
    QString fn = path + app;
    if(proc_isRun(app)) return;
    if(QFile::exists(app)) {
        proc_time(proc);
        proc_md5(proc, fn);
        QString cmd = fn + " &";
        system(cmd.toLatin1().data());
        qDebug() << "process start " +cmd;
        proc_log(app +" startup");
    } else qDebug() << "proc start err:" << fn;
}

void ProcStart::proc_log(const QString &arg)
{
    QString fn = "/tmp/process_log";
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz\t");
    QString str = t + arg; QString fmd = "echo '%1' >> %2 ";
    QString cmd = fmd.arg(str, fn);
    system(cmd.toLatin1().data());
}


QString ProcStart::md5(const QString &fn)
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

bool ProcStart::proc_isRun(const QString &p)
{
    QString cmd = "proc_run " + p; //system("killall proc_run");
    QProcess pro; pro.start(cmd); pro.waitForFinished();
    QByteArray bs = pro.readAllStandardOutput();
    bs +=  pro.readAllStandardError(); mdelay(2);
    return QString::fromLocal8Bit(bs).toInt();
}

void ProcStart::mdelay(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}


