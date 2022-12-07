/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_pwd.h"
#include "pbkdf/sha256.h"

Sercret_Pwd::Sercret_Pwd()
{

}

QByteArray Sercret_Pwd::pwd_encode(const QByteArray &rawData)
{
    uchar temp[16]; char temp1[32]; char password[256]; qstrcpy(password, rawData.data());
    compute_pbkdf2((uint8_t*)password, strlen(password), (uint8_t*)password, strlen(password), 1000, 16, temp);
    print_as_hex_temp(temp, sizeof(temp), temp1); temp1[31] = '\0';
    return temp1;
}
