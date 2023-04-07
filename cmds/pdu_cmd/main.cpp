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
    cout << "*    pduRelaysCtrl   addr   start  num    on"  <<endl;
    cout << "*    pduDataGet      addr   type   topic  sub    id" <<endl;
    cout << "*    pduDataSet      addr   type   topic  sub    id  value" <<endl;
    cout << "*    pduCfgSet       type   fc     value  [id]   [addr] " <<endl;
    cout << "*    pduCfgGet       type   fc     [id]   [addr]" <<endl;
    cout << "*    pduLogFun       type   fc     [id]   [cnt]" <<endl;
    cout << "*    pduLogHda       start  [end]  [addr] [type] [topic] [id]"<<endl;
    cout << "*    pduMetaData     [addr] [dc]" <<endl;
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

void pduDataGet(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 5) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar topic = ls.at(k++).toInt();
        uchar sub = ls.at(k++).toInt();
        uchar id = ls.at(k++).toInt();
        qDebug() << rpc->pduDataGet(addr, type, topic, sub, id);
    } else qCritical() << "pduDataGet Parameter error";
}

void pduDataSet(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    bool res = false; int k = 0; if(ls.size() == 6) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar topic = ls.at(k++).toInt();
        uchar sub = ls.at(k++).toInt();
        uchar id = ls.at(k++).toInt();
        double value = ls.at(k++).toDouble(&res);
        if(res) res = rpc->pduDataSet(addr, type, topic, sub, id, value, 7); //DTxType::TxSsh
        qDebug().noquote() <<  res;
    } else qCritical() << "pduDataSet Parameter error";
}

void pduCfgGet(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() > 1) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        uchar id = 0; if(ls.size() > 2) id = ls.at(k++).toInt();
        uchar addr = 0; if(ls.size() > 3) addr = ls.at(k++).toInt();
        qDebug().noquote() << rpc->pduCfgGet(type, fc, id, addr).toString();
    } else qCritical() << "pduCfgGet Parameter error";
}


void pduCfgSet(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() > 2) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        QString value = ls.at(k++);
        uchar id = 0; if(ls.size() > 3) id = ls.at(k++).toInt();
        uchar addr = 0; if(ls.size() > 4) addr = ls.at(k++).toInt();
        bool res = rpc->pduCfgSet(type, fc, value, id, addr, 7); //DTxType::TxSsh
        qDebug().noquote() << res;
    } else qCritical() << "pduCfgSet Parameter error";
}

void pduMetaData(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    uchar addr = 0; if(ls.size()) addr = ls.first().toInt();
    uchar dc = 0;if(ls.size()>1) dc = ls.at(1).toInt();
    qDebug().noquote() << rpc->pduMetaData(addr, dc);
}

void pduLogFun(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() > 1) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        uchar id = 0; if(ls.size() > 2) id = ls.at(k++).toInt();
        uchar sub = 0; if(ls.size() > 3) sub = ls.at(k++).toInt();
        qDebug() << rpc->pduLogFun(type, fc, id, sub);
    } else qCritical() << "pduLogFun Parameter error";
}

void pduLogHda(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size()) {
        QString start = ls.at(k++);
        QString end; if(ls.size() > 1) end = ls.at(k++);
        uchar addr = 0; if(ls.size() > 2) addr = ls.at(k++).toInt();
        uchar type = 0; if(ls.size() > 3) type = ls.at(k++).toInt();
        uchar topic = 0; if(ls.size() > 4) topic = ls.at(k++).toInt();
        uchar index = 0; if(ls.size() > 5) index = ls.at(k++).toInt();
        qDebug().noquote() << rpc->pduLogHda(start, end, addr, type, topic, index);
    } else qCritical() << "pduLogHda Parameter error";
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
        else if(fc == "pduDataGet") pduDataGet(cmds);
        else if(fc == "pduDataSet") pduDataSet(cmds);
        else if(fc == "pduCfgGet") pduCfgGet(cmds);
        else if(fc == "pduCfgSet") pduCfgSet(cmds);
        else if(fc == "pduRelaysCtrl") pduRelaysCtrl(cmds);
        else if(fc == "pduLogFun") pduLogFun(cmds);
        else if(fc == "pduLogHda") pduLogHda(cmds);
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
