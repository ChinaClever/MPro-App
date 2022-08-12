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
    cout << "*    pduMetaData     addr type  topic sub id" <<endl;
    cout << "*    pduSetData      addr type  topic sub id value" <<endl;
    cout << "*    pduRelaysCtrl   addr start num on" <<endl;
    cout << "*    pduSetParam     addr type  fc  value" <<endl;
    cout << "*    pduGetParam     addr type  fc " <<endl;
    cout << "*    pduLogFun       type fc id cnt" <<endl;
    cout << "*******************************************************" <<endl;
}

QStringList cinGetLine()
{
    QStringList res;
    char buf[128] = {0};
    fprintf(stderr, ">> ");
    std::cin.getline(buf, sizeof(buf),'\n');
    if(strlen(buf)) {
        res = QString(buf).split(" ");
    }
    return res;
}

void pduMetaData(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 5) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar topic = ls.at(k++).toInt();
        uchar sub = ls.at(k++).toInt();
        uchar id = ls.at(k++).toInt();
        qDebug() << rpc->pduMetaData(addr, type, topic, sub, id);
    } else qCritical() << "pduMetaData Parameter error";
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
    int k = 0; if(ls.size() == 3) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        qDebug() << rpc->pduGetParam(addr, type, fc);
    } else qCritical() << "pduGetParam Parameter error";
}


void pduSetParam(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 4) {
        uchar addr = ls.at(k++).toInt();
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        QString value = ls.at(k++);
        qDebug() << rpc->pduSetParam(addr, type, fc, value);
    } else qCritical() << "pduSetParam Parameter error";
}

void pduLogFun(const QStringList &ls)
{
    SshRpcClient *rpc = SshRpcClient::bulid();
    int k = 0; if(ls.size() == 4) {
        uchar type = ls.at(k++).toInt();
        uchar fc = ls.at(k++).toInt();
        uint id = ls.at(k++).toInt();
        uint cnt = ls.at(k++).toInt();
        std::cout << rpc->pduLogFun(type, fc, id, cnt).toStdString();
    } else qCritical() << "pduLogFun Parameter error";
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

bool workDown()
{
    bool ret = true;
    QStringList cmds = cinGetLine();
    if(cmds.size()) {
        QString fc = cmds.takeFirst();
        if(fc == "pduMetaData") pduMetaData(cmds);
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
    bool ret = true; Ssh::usage();

    do {
        ret = Ssh::workDown();
    } while(ret);
    rpc->close();

    return 0;
}
