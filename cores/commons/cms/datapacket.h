#ifndef DATAPACKET_H
#define DATAPACKET_H
#include "cthread.h"

#define LINE_NUM  3
#define LOOP_NUM  12
#define OUTPUT_NUM 48
#define SENOR_NUM 4
#define NAME_SIZE 32
#define DEV_NUM 10
#define ARRAY_SIZE 255    //一包数据最长

#define PACK_ARRAY_SIZE   OUTPUT_NUM

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	10.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	10.0    // 湿度


/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
struct sAlarmUnit
{
    sAlarmUnit() {size=0;}

    uchar size;
    uint value[PACK_ARRAY_SIZE];
    uint rated[PACK_ARRAY_SIZE];

    uint min[PACK_ARRAY_SIZE]; // 最小值
    uint max[PACK_ARRAY_SIZE]; // 最大值
    uchar alarm[PACK_ARRAY_SIZE]; // 报警值 0表示未报警  1表示已报警 2表示已纪录

    uint crMin[PACK_ARRAY_SIZE]; // 最小值
    uint crMax[PACK_ARRAY_SIZE]; // 最大值
};

struct sRelayUnit
{
    uchar size;
    uint sw[PACK_ARRAY_SIZE]; // 开关状态 0 表示未启用
    uchar mode[PACK_ARRAY_SIZE];
    uchar alarm[PACK_ARRAY_SIZE];
    uchar delay[PACK_ARRAY_SIZE];
};


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sObjData
{
    sObjData() {size=0;}
    uchar size;

    sAlarmUnit vol; // 电压
    sAlarmUnit cur; // 电流
    sAlarmUnit pow; // 功率
    sRelayUnit relay;

    uint ele[PACK_ARRAY_SIZE]; // 电能
    uint pf[PACK_ARRAY_SIZE]; // 功率因数

    uint artPow[PACK_ARRAY_SIZE]; // 功率值
    uint reactivePow[PACK_ARRAY_SIZE];
    char name[PACK_ARRAY_SIZE][NAME_SIZE];

    //uint wave[PACK_ARRAY_SIZE]; // 谐波值
    //uint tem[PACK_ARRAY_SIZE];
    //uchar pl[PACK_ARRAY_SIZE]; // 负载百分比
    //uint curThd[PACK_ARRAY_SIZE]; // 电流谐波含量
    //uint volThd[PACK_ARRAY_SIZE]; // 电压谐波含量
};


/**
 * 环境数据结构体
 */
struct sEnvData
{
    sEnvData() {size=0;}
    uchar size;
    uchar type_index;//1:温度 2:湿度 3:门禁 4:门磁 5:水浸 6:烟雾

    char name[SENOR_NUM][NAME_SIZE];
    sAlarmUnit tem; // 温度
    sAlarmUnit hum; // 湿度

    uchar door[SENOR_NUM]; // 门禁
    uchar water[SENOR_NUM]; // 水浸
    uchar smoke[SENOR_NUM]; // 烟雾
};

struct sTgUnit
{
    uint value;
    uint rated;
    ushort min;
    ushort max;

    ushort crMin;
    ushort crMax;
    uchar alarm;
};

struct sTgObjData
{
    sTgUnit vol; // 电压
    sTgUnit cur;  // 电流
    sTgUnit pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint artPow; // 袖在功率
    uint tem;
};


/**
 * RTU传输统计结构体
 */
struct sRtuCount
{
    uint count;
    uint okCount;
    uint errCount;

    uint longCount; // 连续丢命令
    uint longFlag;
};


struct sDevInfo {
    uint devType; //设备类型
    uint devSpec; // 设备规格 A\B\C\D
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uint phases; //设备单三相

    uint version;
    char devName[NAME_SIZE]; // 设备名称
    uint slaveNum;

    uint hz;
    uchar opNum;
    uint outputs;
    uchar ops[DEV_NUM];
    uchar hzs[DEV_NUM];  // 电压频率
    ushort opVers[DEV_NUM];
    uchar chipStates[DEV_NUM];
};

struct sUutInfo {
    char room[NAME_SIZE];
    char module[NAME_SIZE];
    char cab[NAME_SIZE];
    char road[NAME_SIZE];
};

/**
 * 设备数据结构体：
 */
struct sDevData
{
    sDevData() {id=0; offLine=0; alarm=0; }

    uchar id;  // 设备号
    uchar alarm; // 工作状态 ==0 正常
    uchar offLine; //离线标志 > 0在线
    sDevInfo info;
    sUutInfo uut;

    sObjData line; // 相数据
    sObjData loop; // 回路数据
    sObjData output; //位数据
    sTgObjData tg; // 回路数据
    sEnvData env; // 环境数据
    sRtuCount rtuCount; // 传输情况

    uchar lps; // 防雷开关
    uchar dc; // 交直流标志位
    uchar hz; // 电压频率
    ushort br;  // 00	表示波特率9600(00默认9600，01为4800，02为9600，03为19200，04为38400)
};


struct sNetAddr
{
    sNetAddr() {mode=0;}
    uchar mode;
    char ip[NAME_SIZE];
    char mask[NAME_SIZE];
    char gw[NAME_SIZE];
    char dns[NAME_SIZE];
    char mac[NAME_SIZE];
};

struct sDevLogin {
    uchar permit[DEV_NUM];
    char user[DEV_NUM][NAME_SIZE];
    char pwd[DEV_NUM][NAME_SIZE];
};

/**
 * 数据包
 */
struct sDataPacket
{
    sNetAddr net; //设备IP
    sDevData data[DEV_NUM]; //设备数据
    sDevLogin login;
};

#endif // DATAPACKET_H
