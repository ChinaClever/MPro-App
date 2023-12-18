/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_sernum.h"

Op_SerialNumber::Op_SerialNumber(QObject *parent) : OP_ObjCtrl{parent}
{

}


bool Op_SerialNumber::sn_read(uchar addr)
{
    if(m_sn[addr].size()) return true;
    uchar sn[32] = {0}; bool ret = false;
    int len = sn_initReadCmd(addr, sn);
    if(len > 0) {
        ret = sn_analy(sn, len);
        if(ret) sn_toStr(addr);
    }

    return ret;
}


int Op_SerialNumber::sn_initReadCmd(uchar addr, uchar *sn)
{
    uchar buf[] = {0x01, 0x03, 0xA0, 0x01, 0x00, 0x06, 0xB7, 0xCB};
    buf[0] = addr; ushort crc = Crc::Rtu(buf, 6);
    buf[6] = (0xff&crc); buf[7]= (crc >> 8);
    QByteArray rcv = transmit(buf, 8);
    int rtn = 0; if(rcv.size() == 17) {
        for(int i=3; i<17-2; ++i) sn[i] = (uchar)rcv.at(i);
    } else cout << rcv;
    return rtn;
}


void Op_SerialNumber::sn_toStr(uchar addr)
{
    QString cmd; sOpSnItem *it = &mSnItem;
    for(int i=0; i<9; ++i) {cmd += "%" + QString::number(i+1); if(i<7)if((i%2==1)||i==6) cmd += " ";}
    QString sn  = QString(cmd)
                     //.arg(it.devType[0], 2, 16, QLatin1Char('0'))
                     //.arg(it.devType[1], 2, 16, QLatin1Char('0'))
                     .arg(it->devType[2], 2, 16, QLatin1Char('0'))
                     .arg(it->devType[3], 2, 16, QLatin1Char('0'))
                     .arg(it->date[0], 2, 10, QLatin1Char('0'))
                     .arg(it->date[1], 2, 10, QLatin1Char('0'))
                     .arg(it->date[2], 2, 10, QLatin1Char('0'))
                     .arg(it->date[3], 2, 10, QLatin1Char('0'))
                     .arg(it->num, 4, 10, QLatin1Char('0'))
                     .arg(it->pc, 2, 16, QLatin1Char('0'))
                     .arg(it->exor, 2, 16, QLatin1Char('0'));
    m_sn[addr] = sn.toUpper();
}


bool Op_SerialNumber::sn_check(uchar *sn, int len)
{
    bool ret = false;
    if((len != 8) || (sn[0] != 0)  || (sn[1] > 99) ){
        cout << "SN check err" << len << sn[0];
    } else {
        uchar exor = Crc::XorNum(sn, len-1);//暂时注释下面的异或和校验不对
        if(exor == sn[len-1]) ret = true;
        else cout << "SN Crc XorNum err";
    }
    return ret;
}

bool Op_SerialNumber::sn_analy(uchar *sn, int len)
{
    uchar *ptr = sn;
    sOpSnItem *it = &mSnItem;

    uint id = 0;
    for(int i=0; i<4; ++i) {
        id *= 256; id += sn[i];
    }

    for(int i=3; i>=0; --i) {
        it->devType[i] = (0xFF) & (id >> ((3-i)*8));
    }

    for(int i=0; i<4; ++i)  it->date[i] = *ptr++;
    it->num = (*ptr++) << 8;
    it->num += *ptr++;
    it->pc = *ptr++;
    it->exor = *ptr++;

    return sn_check(sn, len);
}
