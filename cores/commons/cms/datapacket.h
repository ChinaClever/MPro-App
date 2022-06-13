#ifndef DATAPACKET_H
#define DATAPACKET_H
#include "cthread.h"

#define LINE_NUM  3
#define LOOP_NUM  6
#define OUTPUT_NUM 48
#define SENOR_NUM 4
#define NAME_SIZE 32
#define DEV_NUM 10
#define ARRAY_SIZE 255    //一包数据最长

#define PACK_ARRAY_SIZE   OUTPUT_NUM

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	100.0    // 电流
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
    uint en[PACK_ARRAY_SIZE];
    uint value[PACK_ARRAY_SIZE];
    uint rated[PACK_ARRAY_SIZE];

    uint min[PACK_ARRAY_SIZE]; // 最小值
    uint max[PACK_ARRAY_SIZE]; // 最大值
    uint alarm[PACK_ARRAY_SIZE]; // 报警值 0表示未报警  1表示已报警 2表示已纪录

    uint crMin[PACK_ARRAY_SIZE]; // 最小值
    uint crMax[PACK_ARRAY_SIZE]; // 最大值
};

struct sRelayUnit
{
    uchar size;
    uint en[PACK_ARRAY_SIZE];
    uint sw[PACK_ARRAY_SIZE]; // 开关状态 0:断开；1:通；2:复位
    uint mode[PACK_ARRAY_SIZE]; // 0 表示未启用  1 表示断开报警
    uint alarm[PACK_ARRAY_SIZE];
    uint delay[PACK_ARRAY_SIZE];
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
    sAlarmUnit pow; // 有功功率
    sRelayUnit relay;

    uint ele[PACK_ARRAY_SIZE]; // 电能
    uint pf[PACK_ARRAY_SIZE]; // 功率因数

    uint artPow[PACK_ARRAY_SIZE]; // 视在功率
    uint reactivePow[PACK_ARRAY_SIZE]; // 无功功率
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

    uint door[SENOR_NUM]; // 门禁
    uint water[SENOR_NUM]; // 水浸
    uint smoke[SENOR_NUM]; // 烟雾
};

struct sTgUnit
{
    uint en;
    uint value;
    uint rated;
    uint min;
    uint max;

    uint crMin;
    uint crMax;
    uint alarm;
};

struct sTgObjData
{
    sTgUnit vol; // 电压
    sTgUnit cur;  // 电流
    sTgUnit pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint artPow; // 袖在功率
    uint reactivePow;
};


/**
 * RTU传输统计结构体
 */
struct sRtuCount
{
    uchar offLines[4];
};


struct sDevInfo {
    uint devSpec; // 设备规格 A\B\C\D
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra

    uint version;
    uint slaveNum;  // 副机数量
    uchar modbusAddr; // 通讯地址
    uchar buzzerSw; // 蜂鸣器开关
    uchar drySw; // 报警干接点开关

    uint hz;
    uint lineNum; //设备单三相
    uint opNum;   //　执行板数量
    uint loopNum; // 回路数量
    uint outputNum;   //　输出位数量
    uchar ops[DEV_NUM]; //　每块执行板的输出位数量
    uchar loopEnds[LOOP_NUM];
    uchar loopStarts[LOOP_NUM];

    uchar hzs[DEV_NUM];  // 电压频率
    ushort opVers[DEV_NUM]; // 每块执行板软件版本
    uchar chipStates[DEV_NUM];
    uchar reserve[20];
};

struct sUutInfo {
    char idc[NAME_SIZE];
    char room[NAME_SIZE];
    char module[NAME_SIZE];
    char cab[NAME_SIZE];
    char road[NAME_SIZE];
    char devName[NAME_SIZE]; // 设备名称
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

    sObjData line; // 相数据
    sObjData loop; // 回路数据
    sObjData output; //位数据
    sTgObjData tg; // 回路数据
    sEnvData env; // 环境数据
    sRtuCount rtuCount; // 传输情况

    sDevInfo info;
    sUutInfo uut;

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

enum DType{Tg, Line, Loop, Output, Env=6, Sensor};
enum DTopic{Relay=1, Vol, Cur, Pow, Ele, PF, ArtPow, ReactivePow, Tem=11, Hum, Door1=21, Door2, Water, Smoke};
enum DSub{Size, Value, Rated, Alarm, VMax, VMin, VCrMin, VCrMax, EnAlarm};
enum AlarmStatus{Ok, Min=1, CrMin=2, CrMax=4, Max=8};
enum DTxType{Tx, TxWeb, TxModbus, TxSnmp, TxRpc, TxUdp, TxTcp, TxWebocket,TxSsh};

struct sDataItem
{
    sDataItem():addr(0),rw(0),value(0){}
    uchar addr; // 地址
    uchar type; // 1 相数据  2 回路数据 ３　输出位数据  6 环境 7 传感器
    uchar topic; // 1 开关  2 电压  3 电流  4 功率  6温度 7湿度
    uchar subtopic;  // 0 Size 1 当前值 2 额定值 3 报警状态
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar id; // 0 表示统一设置
    uchar rw; // 0 读  1 写
    uint value;
};

enum SFnCode{OutputName=10, Uuts,ECfgNum, EDevInfo};

struct sNumStrItem{
    sNumStrItem():addr(0),isDigit(0),rw(0),value(0){}
    uchar addr; // 地址
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar isDigit; // 0 字符串 1 数字
    uchar fc; // 10 输出位  11 UUT信息
    uchar id; // 功能id　0 表示统一设置
    uchar rw; // 0 读  1 写
    uint value;
    char str[NAME_SIZE];
};

struct sRelay
{
    enum State{Off, On, Reset};
    enum Type{Breaker, Relay};
    enum Mode{Standard, EnOffALarm};
    enum Alarm{NoAlarm, OffALarm};
};


#endif // DATAPACKET_H
