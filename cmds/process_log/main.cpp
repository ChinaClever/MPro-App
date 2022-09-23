/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include <QtCore>

bool appendFile(const QString &msg)
{
    QString fn = "usr/data/clever/cfg/process_log.txt";
#if (QT_VERSION > QT_VERSION_CHECK(5,13,0))
    fn = "process_log.txt";
#endif
    QFile file(fn);
    if(file.open(QIODevice::Append | QIODevice::Text)) {
        QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz\t");
        QByteArray array = t.toUtf8() + msg.toUtf8()+"\n";
        file.write(array);
    } file.close();

    return true;
}


int main(int argc, char *argv[])
{
    if(argc > 1) appendFile(argv[1]);
    else qDebug() << "error";
    return 0;
}
