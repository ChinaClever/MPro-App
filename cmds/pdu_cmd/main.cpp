/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include <QCoreApplication>
#include "sshrpcclient.h"
#include <iostream>

namespace Ssh{
void usage()
{
    using namespace std;
    cout << "*******************************************************" <<endl;
    cout << "* usage: of pdu_cmd" <<endl;
    cout << "*    pduRelaysCtrl   addr  start  num    on"  <<endl;
    cout << "*    pduGetData      addr  type   topic  sub    id" <<endl;
    cout << "*    pduSetData      addr  type   topic  sub    addr  value" <<endl;
    cout << "*    pduSetParam     type  fc     value  [addr] [sub]" <<endl;
    cout << "*    pduGetParam     type  fc     [addr] [sub]" <<endl;
    cout << "*    pduLogFun       type  fc     [id]   [cnt]" <<endl;
    cout << "*    pduMetaData     [addr]  " <<endl;
    cout << "*******************************************************" <<endl;
}

QStringList cinGetLine()
{
    QStringList res;
    fprintf(stderr, ">> ");
    static char buf[1024] = {0};
    memset(buf, 0, sizeof(buf));
    std::cin.getline(buf, sizeof(buf),'\n');
    res = QString(buf).simplified().split(" ");
    return res;
}

void pduGetData(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 5) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar topic = ls.at(k++).toInt();
        uchar sub = ls.at(k++).toInt();
        uchar id = ls.at(k++).toInt();
        qDebug() << rpc->pduGetData(addr, type, topic, sub, id);
    } else qCritical() << "pduGetData Parameter error";
}

void pduSetData(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 6) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar topic = ls.at(k++).toInt();
        uchar sub = ls.at(k++).toInt();
        uchar id = ls.at(k++).toInt();
        uint value = ls.at(k++).toInt();
        qDebug() << rpc->pduSetData(addr, type, topic, sub, id, value);
    } else qCritical() << "pduSetData Parameter error";
}


void pduGetParam(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() > 1) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        uchar addr = 0; if(ls.size() > 2) addr = ls.at(k++).toInt();
        uchar sub = 0; if(ls.size() > 3) sub = ls.at(k++).toInt();
        qDebug() << rpc->pduGetParam(type, fc, addr, sub).toString();
    } else qCritical() << "pduGetParam Parameter error";
}


void pduSetParam(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() > 2) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        QString value = ls.at(k++);
        uchar addr = 0; if(ls.size() > 3) addr = ls.at(k++).toInt();
        uchar sub = 0; if(ls.size() > 4) sub = ls.at(k++).toInt();
        qDebug() << rpc->pduSetParam(type, fc, value, addr, sub);
    } else qCritical() << "pduSetParam Parameter error";
}

void pduMetaData(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    uchar addr = 0; if(ls.size()) addr = ls.first().toInt();
    std::cout << rpc->pduMetaData(addr).toStdString() << std::endl;
}

void pduLogFun(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    qDebug() << QTime::currentTime().toString("hh:mm:ss zzz");
    int k = 0; if(ls.size() > 2) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        uchar id = 0; if(ls.size() > 2) id = ls.at(k++).toInt();
        uchar sub = 0; if(ls.size() > 3) sub = ls.at(k++).toInt();
        std::cout << rpc->pduLogFun(type, fc, id, sub).toStdString() << std::endl;
    } else qCritical() << "pduLogFun Parameter error";
    qDebug() << QTime::currentTime().toString("hh:mm:ss zzz");
}

void pduRelaysCtrl(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 4) {
        uchar addr = ls.at(k++).toInt();
        uchar start = ls.at(k++).toInt();
        uchar num = ls.at(k++).toInt();
        uchar on = ls.at(k++).toInt();
        qDebug() << rpc->pduRelaysCtrl(addr, start, num, on);
    } else qCritical() << "pduRelaysCtrl Parameter error";
}


bool workDown(const QStringList &str)
{
    int ret = true;
    QStringList cmds = str;
    if(cmds.size()) {
        QString fc = cmds.takeFirst();
        if(fc == "pduMetaData") pduMetaData(cmds);
        else if(fc == "pduGetData") pduGetData(cmds);
        else if(fc == "pduSetData") pduSetData(cmds);
        else if(fc == "pduGetParam") pduGetParam(cmds);
        else if(fc == "pduSetParam") pduSetParam(cmds);
        else if(fc == "pduRelaysCtrl") pduRelaysCtrl(cmds);
        else if(fc == "pduLogFun") pduLogFun(cmds);
        else if(fc == "quit") ret = false;
        else usage();
    }
    return ret;
}
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QObject *p = a.parent();
    SshRpcClient *rpc = SshRpcClient::bulid(p);
    if(argc > 1) {
        QStringList cmd;
        for(int i=1; i<argc; i++) cmd << argv[i];
        Ssh::workDown(cmd);
    } else {
        bool ret = true;
        Ssh::usage();
        do {
            QStringList cmd = Ssh::cinGetLine();
            if(cmd.size()) ret = Ssh::workDown(cmd);
        } while(ret);
    }

    rpc->close();

    return 0;
}
