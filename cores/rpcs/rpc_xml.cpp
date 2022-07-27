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
    mXml->addMethod("pduSetData", rpc, "pduSetData");
    mXml->addMethod("pduMetaData", rpc, "pduMetaData");
    mXml->addMethod("pduGetString", rpc, "pduGetString");
    mXml->addMethod("pduSetString", rpc, "pduSetString");
    mXml->addMethod("pduDevCfg", rpc, "pduDevCfg");
    mXml->addMethod("pduSetCfg", rpc, "pduSetCfg");
    mXml->addMethod("pduLogFun", rpc, "pduLogFun");

    mXml->addMethod("pduRelayCtrl", rpc, "pduRelayCtrl");
    mXml->addMethod("pduGroupCtrl", rpc, "pduGroupCtrl");
    mXml->addMethod("pduRelayStatus", rpc, "pduRelayStatus");
    mXml->addMethod("pduRelayModeSet", rpc, "pduRelayModeSet");
    mXml->addMethod("pduRelayDelaySet", rpc, "pduRelayDelaySet");

    mXml->addMethod("pduRelaysCtrl", rpc, "pduRelaysCtrl");
    mXml->addMethod("pduGroupingSet", rpc, "pduGroupingSet");
    mXml->addMethod("pduGroupNameSet", rpc, "pduGroupNameSet");
    mXml->addMethod("pduOutputNameSet", rpc, "pduOutputNameSet");
    mXml->addMethod("pduOutputNameGet", rpc, "pduOutputNameGet");
    mXml->addMethod("pduGroupNameGet", rpc, "pduGroupNameGet");
    mXml->addMethod("pduGroupingGet", rpc, "pduGroupingGet");

    mXml->addMethod("pduQRcodeGenerator", rpc, "pduQRcodeGenerator");
    mXml->addMethod("pduDevNameGet", rpc, "pduDevNameGet");
    mXml->addMethod("pduDevNameSet", rpc, "pduDevNameSet");
    mXml->addMethod("pduDevSnGet", rpc, "pduDevSnGet");
    mXml->addMethod("pduDevSnSet", rpc, "pduDevSnSet");
}
