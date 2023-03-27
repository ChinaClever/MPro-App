/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_rpc.h"

Web_Obj *Web_Rpc::mObj = nullptr;
Web_Rpc::Web_Rpc(QObject *parent) : QObject{parent}
{
    mObj = Web_Obj::bulid(parent);
}

char* Web_Rpc::pduReadData(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5); //cout << its;
    double value = mObj->getValue(its.at(0), its.at(1), its.at(2),its.at(3), its.at(4));
    return responRpcData(its, value);
}

char *Web_Rpc::pduMetaData(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5); //cout << its;
    QString value = mObj->metaData(its.at(0));
    return responRpcString(its, value);
}

char *Web_Rpc::execute(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5);
    QString value = mObj->getString(r, 5);
    value = mObj->execute(value);
    return responRpcString(its, value);
}

char* Web_Rpc::pduSetData(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5);
    QVariant value = mObj->getString(r, 5);
    if(value.toString().isNull()) value = mObj->getNumber(r, 5);
    //cout << its << value;  ////////===============
    //double value = mObj->getString(r, 5).toDouble();  cout << its << value;
    mObj->setting(its.at(0), its.at(1), its.at(2), its.at(3), its.at(4), value.toDouble());
    return responRpcData(its, value.toDouble()); // ret?1:0
}

char* Web_Rpc::responRpcData(const QVector<uint> &ls, double value)
{
    QVector<double> its; foreach(auto &i, ls) its << i;
    return mg_mprintf("[%g,%g,%g,%g,%g,%g]", its.at(0), its.at(1), its.at(2),its.at(3), its.at(4), value);

}

char* Web_Rpc::responRpcString(const QVector<uint> &ls, const QString &value)
{
    QVector<double> its; foreach(auto &i, ls) its << i;
    return mg_mprintf("[%g,%g,%g,%g,%g,%Q]" , its.at(0), its.at(1), its.at(2),
                      its.at(3), its.at(4), value.toStdString().c_str());
}

char *Web_Rpc::pduReadParam(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5); //cout << its;
    QString value = mObj->getCfg(its.at(1), its.at(2), its.at(3), its.at(0));
    return responRpcString(its, value);
}

char *Web_Rpc::pduSetParam(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5);
    QVariant value = mObj->getString(r, 5);
    if(value.toString().isNull()) value = mObj->getNumber(r, 5); //cout << its << value;
    int ret = mObj->setCfg(its.at(1), its.at(2), value,its.at(3), its.at(0));
    if(its.at(1) == 14 && its.at(2) == 11) value = ret; //?1:0;
    return responRpcString(its, value.toString());
}

char* Web_Rpc::pduLogFun(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 5);
    QString value = mObj->log_fun(its.at(2), its.at(3),its.at(0), its.at(4)).toString();
    return responRpcString(its, value);
}

char* Web_Rpc::pduLogHda(mg_str &r)
{
    QVector<uint> its = mObj->getNumbers(r, 2);
    QString vs = mObj->getString(r, 2);
    QString start = mObj->getString(r, 3);
    QString end = mObj->getString(r, 4);

    QStringList ls = vs.split(";"); int k = 0;
    uchar type = 0; if(ls.size() > 0) type = ls.at(k++).toInt();
    uchar topic = 0; if(ls.size() > 1) topic = ls.at(k++).toInt();
    uchar index = 0; if(ls.size() > 2) index = ls.at(k++).toInt();
    QString value = mObj->log_hda(start, end, its.at(0), type, topic, index);
    its << type << topic << index;
    return responRpcString(its, value);
}
