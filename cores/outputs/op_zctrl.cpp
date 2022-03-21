#include "op_zctrl.h"

OP_ZCtrl::OP_ZCtrl(QObject *parent) : OP_ObjCtrl{parent}
{

}

void OP_ZCtrl::writeCtrlCmd(uchar *cmd, int k)
{
    sDevData *dev = &(cm::dataPacket()->data[0]);
    for(int i=0; i<3; ++i) cmd[k++] = dev->info.ops[i];  // 三块执行板各个输出位个数

    cmd[k++] = 0x44;
    cmd[k] = cm::xorNum(cmd,sizeof(cmd)-1);
    if(!isOta) {
        waitForLock();
        writeSerial(cmd, sizeof(cmd));
    }
}

void OP_ZCtrl::funSwitch(uchar *on, uchar *off)
{
    int k = 5, all = 1; // mItem->addr
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = on[i];  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = off[i];  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;
    for(int i=0; i<6; ++i) if((on[i] != 0xFF) || (off[i] != 0xFF)) all = 0;
    cmd[k++] = all; // 统一开关为1
    writeCtrlCmd(cmd, k);
}

void OP_ZCtrl::funClearEle(uchar *buf)
{
    int k = 5;
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x00, 0xA4, 0xB4};
    for(int i=0; i<6; i++) cmd[k++] = buf[i];
    for(int i=0; i<52; i++) cmd[k++] = 0x00;  //预留位
    writeCtrlCmd(cmd, k);
}


void OP_ZCtrl::funDelay(uchar *buf)
{
    int k = 5;
    uchar cmd[zCmdLen] = {0x7B, 0xC1, 0x00, 0xA3, 0xB3};

    for(int i=0; i<42; i++) cmd[k++] = buf[i];
    for(int i=0; i<8; i++) cmd[k++] = 0x00;  // 组
    for(int i=0; i<8; i++) cmd[k++] = 0x00; //预留位
    writeCtrlCmd(cmd, k);
}