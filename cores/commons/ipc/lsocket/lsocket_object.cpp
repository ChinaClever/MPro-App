/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "lsocket_object.h"

LSocket_Object::LSocket_Object(QObject *parent) : SM_Obj{parent}
{

}

void LSocket_Object::setKey(const QString &newKey)
{
    mKeyObj = newKey;
}

const QString &LSocket_Object::key() const
{
    return mKeyObj;
}
