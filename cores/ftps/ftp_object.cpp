/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_object.h"

sFtpCfg Ftp_Object::ftpCfg;
Ftp_Object::Ftp_Object()
{
    system("mkdir -p /tmp/csv");
    system("mkdir -p /tmp/xlsx");
}
