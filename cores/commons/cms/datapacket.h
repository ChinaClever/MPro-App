#ifndef DATAPACKET_H
#define DATAPACKET_H
#ifndef SUPPORT_C
#include "cthread.h"
#else
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
#endif

#define NET_NUM 2
#define LINE_NUM  3
#define LOOP_NUM  6
#define OUTPUT_NUM 48
#define SENOR_NUM 4
#define NAME_SIZE 32
#define DEV_NUM 10
#define ARRAY_SIZE 255    //一包数据最长
#define USER_NUM 5
#define GROUP_NUM 8
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
#ifndef SUPPORT_C
    sAlarmUnit() {size=0;}
#endif
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
    uint alarm[PACK_ARRAY_SIZE]; // 报警状态
    uint delay[PACK_ARRAY_SIZE]; // 上电延时
    uint resTime[PACK_ARRAY_SIZE]; // 复位延时
};


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sObjData
{
#ifndef SUPPORT_C
    sObjData() {size=0;}
#endif
    uchar size;

    struct sAlarmUnit vol; // 电压
    struct sAlarmUnit cur; // 电流
    struct sAlarmUnit pow; // 有功功率
    struct sRelayUnit relay;

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
#ifndef SUPPORT_C
    sEnvData() {size=0;}
#endif
    uchar size;
    struct sAlarmUnit tem; // 温度
    struct sAlarmUnit hum; // 湿度

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
    struct sTgUnit vol; // 电压
    struct sTgUnit cur;  // 电流
    struct sTgUnit pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint artPow; // 袖在功率
    uint reactivePow;
};


/**
 * RTU执行板结构体
 */
struct sRtuBoard
{
    uchar hzs[DEV_NUM];  // 电压频率
    uchar offLines[DEV_NUM];
    uchar chipStates[DEV_NUM];
    ushort br;  // 00	表示波特率9600(00默认9600，01为4800，02为9600，03为19200，04为38400)
};

struct sDevNums
{
    uint slaveNum;  // 副机数量
    uint lineNum; //设备单三相
    uint boardNum;   //　执行板数量
    uint loopNum; // 回路数量
    uint outputNum;   //　输出位数量
    uchar boards[DEV_NUM]; //　每块执行板的输出位数量
    uchar loopEnds[LOOP_NUM]; //
    uchar loopStarts[LOOP_NUM];
    uchar boardSpecs[LOOP_NUM];  // 各执行板的规格
    uchar group[GROUP_NUM][OUTPUT_NUM];
    uint groupEn; // 组开关使能
};

struct sVersions
{
    uint core;
    char coreVer[NAME_SIZE];
    char coreCompileTime[NAME_SIZE];

    uint web;
    char webVer[NAME_SIZE];
    char webCompileTime[NAME_SIZE];

    uint lcd;
    char lcdVer[NAME_SIZE];
    char lcdCompileTime[NAME_SIZE];

    uint start;
    char startVer[NAME_SIZE];
    char startCompileTime[NAME_SIZE];

    ushort opVers[DEV_NUM]; // 每块执行板软件版本
    uint version;
};

struct sUutInfo {
    char idc[NAME_SIZE];
    char room[NAME_SIZE];
    char module[NAME_SIZE];
    char cab[NAME_SIZE];
    char road[NAME_SIZE];
    char devName[NAME_SIZE]; // 设备名称
    char qrcode[2*NAME_SIZE]; // 二维码
    char sn[NAME_SIZE];
};

struct sParameter {
    uint devSpec; // 设备规格 A\B\C\D
    uchar language; // 0 中文 1 英文
    uchar modbusAddr; // 通讯地址
    uchar buzzerSw; // 蜂鸣器开关
    uchar drySw; // 报警干接点开关
    uint runTime; // 最近开关运行时间 分钟为单位
    uint uptime; // 持续运行时间 单位小时
    uint hz; // 产品实时频繁

    uchar reserve[20];
};

struct sDevCfg {
    struct sUutInfo uut;
    struct sDevNums nums;
    struct sVersions vers;
    struct sParameter param;
};

/**
 * 设备数据结构体：
 */
struct sDevData
{
#ifndef SUPPORT_C
    sDevData() {id=0; offLine=0; alarm=0; }
#endif

    uchar id;  // 设备号
    uchar alarm; // 工作状态 ==0 正常
    uchar offLine; //离线标志 > 0在线
    struct sObjData line; // 相数据
    struct sObjData loop; // 回路数据
    struct sObjData group; //组数据
    struct sObjData output; //位数据
    struct sTgObjData tg; // 回路数据
    struct sEnvData env; // 环境数据
    struct sRtuBoard rtu; // 执行板
    struct sDevCfg cfg; // 配置数据

    uchar lps; // 防雷开关
    uchar dc; // 交直流标志位
    uint hz; // 电压频率
};


struct sNetAddr
{
#ifndef SUPPORT_C
    sNetAddr() {mode=0;}
#endif
    uchar mode;
    char mac[NAME_SIZE];
    char ip[NAME_SIZE];
    char mask[NAME_SIZE];
    char gw[NAME_SIZE];
    char dns[NAME_SIZE];

    char ipv6[NAME_SIZE];
    char gwv6[NAME_SIZE];
    char dnsv6[NAME_SIZE];
    uint maskv6[NET_NUM];
};

struct sDevLogin {
    char permit[3];
    char token[NAME_SIZE];
    char user[NAME_SIZE];
    char pwd[NAME_SIZE];
};

/**
 * 数据包
 */
struct sDataPacket
{
    struct sNetAddr net[NET_NUM]; //设备IP
    struct sDevData data[DEV_NUM]; //设备数据
    struct sDevLogin login[USER_NUM];
};


enum DType{Tg, Line, Loop, Output, Group, Env=6, Sensor};
enum DTopic{Relay=1, Vol, Cur, Pow, Ele, PF, ArtPow, ReactivePow, Tem=11, Hum, Door1=21, Door2, Water, Smoke};
enum DSub{Size, Value, Rated, Alarm, VMax, VMin, VCrMin, VCrMax, EnAlarm, UpTime=4, ResTime, Relays=11};
enum AlarmStatus{Ok, Min=1, CrMin=2, CrMax=4, Max=8};
enum DTxType{Tx, TxWeb, TxModbus, TxSnmp, TxRpc, TxJson, TxWebocket,TxSsh};

struct sDataItem
{
#ifndef SUPPORT_C
    sDataItem():addr(0),rw(0),value(0){}
#endif
    //uchar soi; // 0 本机 1 级联组 2 本机房 3 所有
    uchar addr; // 地址
    uchar type; // 1 相数据  2 回路数据 ３　输出位数据 4组数据 6 环境 7 传感器
    uchar topic; // 1 开关  2 电压  3 电流  4 功率  6温度 7湿度
    uchar subtopic;  // 0 Size 1 当前值 2 额定值 3 报警状态  11 多开关控制
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar id; // 0 表示统一设置
    uchar rw; // 0 读  1 写
    uint value;
};

enum SFnCode{OutputName=10, Uuts, ECfgNum, EDevInfo, EDevLogin, EModbus, ESnmp, ERpc, EPush,
             EGrouping=21,EGroupName};

struct sCfgItem {
#ifndef SUPPORT_C
    sCfgItem():addr(0),sub(0){}
#endif
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar addr; // 地址
    uchar type; // 10 输出位  11 UUT信息
    uchar fc; // 功能码　0 表示统一设置
    uchar sub;
};

#ifndef SUPPORT_C
struct sCfgUnit {
    sCfgItem it;
    QVariant v;
};

struct sRelay
{
    enum Type{Breaker, Relay};
    enum Mode{Standard, EnOffALarm};
    enum Alarm{NoAlarm, OffALarm};
    enum State{Off, On, Reset};
};
#endif

#endif // DATAPACKET_H
