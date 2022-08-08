/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_xml.h"

Rpc_Xml::Rpc_Xml(QObject *parent)
    : QObject{parent}
{    
    mXml = new MaiaXmlRpcServer(this);
    addMethods();
}

bool Rpc_Xml::startXmlRpc(int en, int port)
{
    bool ls = mXml->isListening();
    if(en) {
        if(ls) mXml->close();
        else ls = mXml->listen(QHostAddress::Any, port);
    } else {
        if(ls) mXml->close();
    }
    return ls;
}

void Rpc_Xml::addMethods()
{
    Rpc_Method *rpc = Rpc_Method::bulid(this);
    mXml->addMethod("pduLogFun", rpc, "pduLogFun");
    mXml->addMethod("pduSetData", rpc, "pduSetData");
    mXml->addMethod("pduMetaData", rpc, "pduMetaData");
    mXml->addMethod("pduSetParam", rpc, "pduSetParam");
    mXml->addMethod("pduGetParam", rpc, "pduGetParam");
    mXml->addMethod("pduRelaysCtrl", rpc, "pduRelaysCtrl");
}
