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

#define LINE_NUM  3
#define LOOP_NUM  6
#define OUTPUT_NUM 48
#define SENOR_NUM 4
#define NAME_SIZE 48
#define DEV_NUM 20
#define ARRAY_SIZE 255    //一包数据最长
#define USER_NUM 10
#define GROUP_NUM 8
#define PACK_ARRAY_SIZE   (OUTPUT_NUM+12)

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	100.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	10.0    // 温度
#define COM_RATE_HUM	1.0    // 湿度

#define COM_MIN_VOL     (50*COM_RATE_VOL)
#define COM_MAX_VOL     (600*COM_RATE_VOL)
#define COM_MAX_CUR     (100*COM_RATE_CUR)
#define COM_MAX_POW     (100*600)

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
struct sAlarmUnit
{
#ifndef SUPPORT_C
    sAlarmUnit() {size=0;}
#endif
    int size;
    uint en[PACK_ARRAY_SIZE]; // 报警开启
    uint cnt[PACK_ARRAY_SIZE]; // 连续报警次数
    uint hda[PACK_ARRAY_SIZE]; // 数据记录开关
    uint value[PACK_ARRAY_SIZE]; // 当前值
    uint rated[PACK_ARRAY_SIZE]; // 额定值

    uint min[PACK_ARRAY_SIZE]; // 最小值
    uint max[PACK_ARRAY_SIZE]; // 最大值
    uint alarm[PACK_ARRAY_SIZE]; // 报警值 0表示未报警  1和8表示已报警 2和4表示预警

    uint crMin[PACK_ARRAY_SIZE]; // 最小值
    uint crMax[PACK_ARRAY_SIZE]; // 最大值
    uint peakMax[PACK_ARRAY_SIZE]; // 最大峰值
    uint peakStamp[PACK_ARRAY_SIZE]; // 峰值的时间戳
    uint reserve[6][PACK_ARRAY_SIZE]; // 预留
};

struct sRelayUnit
{
    int size;
    uint en[PACK_ARRAY_SIZE];
    uint sw[PACK_ARRAY_SIZE]; // 开关状态 0:断开；1:通；2:复位
    uint cnt[PACK_ARRAY_SIZE]; // 继电器控制次数
    uint offAlarm[PACK_ARRAY_SIZE]; // 0 表示未启用  1 表示断开报警
    uint alarm[PACK_ARRAY_SIZE]; // 报警状态
    uint powerUpDelay[PACK_ARRAY_SIZE]; // 上电延时
    uint resetDelay[PACK_ARRAY_SIZE]; // 复位延时
    uint overrunOff[PACK_ARRAY_SIZE]; // 超限断电
    uint timingEn[PACK_ARRAY_SIZE]; // 定时开关
    char timingOn[PACK_ARRAY_SIZE][NAME_SIZE];
    char timingOff[PACK_ARRAY_SIZE][NAME_SIZE];
    uint reserve[6][PACK_ARRAY_SIZE]; // 预留
};


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sObjData
{
#ifndef SUPPORT_C
    sObjData() {size=0;}
#endif
    int size;
    struct sAlarmUnit vol; // 电压
    struct sAlarmUnit cur; // 电流
    struct sAlarmUnit pow; // 有功功率
    struct sRelayUnit relay;

    uint ele[PACK_ARRAY_SIZE]; // 电能
    uint pf[PACK_ARRAY_SIZE]; // 功率因数

    uint artPow[PACK_ARRAY_SIZE]; // 视在功率
    uint hdaEle[PACK_ARRAY_SIZE]; // 电能数据记录开关
    uint reactivePow[PACK_ARRAY_SIZE]; // 无功功率
    char name[PACK_ARRAY_SIZE][NAME_SIZE];
    uint lineVol[PACK_ARRAY_SIZE]; // 线电压
    uint reserve[6][PACK_ARRAY_SIZE]; // 预留

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
    uint size;
    uint isInsert[SENOR_NUM];
    struct sAlarmUnit tem; // 温度
    struct sAlarmUnit hum; // 湿度

    uint wind[SENOR_NUM]; // 风速
    uint door[SENOR_NUM]; // 门禁
    uint water[SENOR_NUM]; // 水浸
    uint smoke[SENOR_NUM]; // 烟雾
    uint reserve[10][SENOR_NUM]; // 预留
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
    uint reserve[10]; // 预留
};

struct sTgObjData
{
    struct sTgUnit vol; // 电压
    struct sTgUnit cur;  // 电流
    struct sTgUnit pow; // 功率

    uint ele; // 电能
    uint pf; // 功率因数
    uint artPow; // 视在功率
    uint leakCur; // 漏电电流
    uint reactivePow; // 无功功率
    uint reserve[10]; // 预留
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
    uint reserve;
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
    uint reserve[20];
};

struct sVersions
{
    char usr[NAME_SIZE]; // 客户名称
    char md5[NAME_SIZE]; // 校验码
    char ver[NAME_SIZE]; // 版本号
    char dev[NAME_SIZE]; // 设备类型
    char remark[4*NAME_SIZE]; // 发布说明
    char oldVersion[NAME_SIZE]; // 旧版本号
    char compileDate[NAME_SIZE]; // 编译时间
    char releaseDate[NAME_SIZE]; // 发布时间
    char upgradeDate[NAME_SIZE]; // 升级时间
    char serialNumber[NAME_SIZE]; // 序列号
    char hwVersion[NAME_SIZE]; // 硬件版本
    char reserve[3][NAME_SIZE]; // 预留
    uint opVers[DEV_NUM]; // 每块执行板软件版本
};

struct sUutInfo {
    char room[NAME_SIZE];
    char location[NAME_SIZE]; // 位置
    char devName[NAME_SIZE]; // 设备名称
    char qrcode[4*NAME_SIZE]; // 二维码
    char devType[NAME_SIZE]; // 设备型号
    char reserve[3][NAME_SIZE];
    char sn[NAME_SIZE];
};

struct sParameter {
    uint devSpec; // 设备规格 A\B\C\D
    uint runStatus; // 运行状态 0：正常 1：告警 2：故障 3: 离线
    uint language; // 0 中文 1 英文
    uint devMode; // 0：标准 1：级联 2：机柜双电源 3：RTU
    uint cascadeAddr; // 级联地址
    uint  modbusRtuBr; // Modbus-Rtu 波特率
    uint modbusRtuAddr; // Modbus-Rtu 地址
    uint buzzerSw; // 蜂鸣器开关
    uint  buzzerSec; // 消音时间
    uint drySw; // 报警干接点开关
    uint isBreaker; // 0没有断路器 1有断路器
    uint screenAngle; // 屏幕方位角
    uint backlightType; // 屏幕显示模式 0 常亮 1 节能
    uint backlightTime; // 节能 时长 小时数
    uint groupEn; // 组开关使能
    uint sensorBoxEn; // 传感器盒子 0：禁用， 1：启用
    uint powLogEn; // 总功率记录功能是否启用 0：禁用， 1：启用
    uint jsonContent; // 0：自动 1：最多 2：最少
    uint runTime; // 总运行时间 单位天
    uint vh; // 0:垂直 1:水平
    uint hz; // 产品实时频繁
    uint jsonCompress; // 0：不压缩 1：压缩
    uint reserve[19];
};

struct sRunTime
{
    uint runSec; // 最近开关运行时间 秒钟为单位
    uint resetCnt; // 重启次数；
    uint daemonSec; // 守护进程使用
    char md5[NAME_SIZE]; // 运行程序的ＭＤ５值
    char start[NAME_SIZE]; // 启动时间
    char compileTime[NAME_SIZE]; // 编译时间
    char reserve[3][NAME_SIZE];
};

struct sProcState
{
    struct sRunTime daemon; // 守护进程
    struct sRunTime core; // 主程序启动时间
    struct sRunTime ota; // 升级启动时间
    struct sRunTime web; // 网页启动时间
    struct sRunTime awtk; // 屏幕启动时间
    struct sRunTime reserve;
};

struct sDevCfg {
    struct sUutInfo uut;
    struct sDevNums nums;
    struct sVersions vers;
    struct sParameter param;
};

struct sFaultCode {
    uint fault; // 是否在故障
    uint cnt[4][PACK_ARRAY_SIZE];
    uint code[PACK_ARRAY_SIZE];
    uint reserve[10];
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
    uchar status; // 0：正常 1：告警 2：故障 3: 离线
    uchar offLine; //离线标志 > 0在线
    struct sObjData line; // 相数据
    struct sObjData loop; // 回路数据
    struct sObjData group; //组数据
    struct sObjData output; //位数据
    struct sObjData dual; //双电源
    struct sTgObjData tg; // 统计数据
    struct sEnvData env; // 环境数据
    struct sRtuBoard rtu; // 执行板
    struct sDevCfg cfg; // 配置数据
    struct sFaultCode dtc; // 故障码
    struct sProcState proc; // 进程状态

    uchar lps; // 防雷开关
    uchar dc; // 交直流标志位
    uint hz; // 电压频率
    uint reserve[10];
};


struct sNetAddr
{
#ifndef SUPPORT_C
    sNetAddr() {en = dhcp=0;}
#endif    
    uchar en, dhcp;
    char ip[NAME_SIZE];
    char gw[NAME_SIZE];
    char mask[NAME_SIZE];
    char dns[NAME_SIZE];
    char dns2[NAME_SIZE];
    char reserve[3][NAME_SIZE];
    uchar prefixLen;
};

struct sNetInterface
{
    struct sNetAddr inet;
    struct sNetAddr inet6;
    char mac[NAME_SIZE];
    char name[NAME_SIZE];
};

struct sDevLogin
{
    uchar permit; // 0 管理员  1 操作员  2 访客
    uint groupCtrl; // 按位操作 0 有权限  1 没有权限
    uchar ctrl[GROUP_NUM];
    char token[NAME_SIZE];
    char user[NAME_SIZE];
    char pwd[NAME_SIZE];
    char reserve[4][NAME_SIZE];
};

struct sOtaUpIt
{
    uchar isRun; // 0 完成 1 进行中  2 失败
    uchar subId; // 升级子对象 第几个副机，或者第几块执板
    uchar progress; // 升级进度 百分之几十
    uchar reserve;
    uchar progs[DEV_NUM]; // 升级进度
    uchar results[DEV_NUM]; // 升级结果
};

struct sOtaUpdater
{
    uint work; // 按位操作：0 无升级 １Ｕ盘升级 2 网络升级 ３　网页升级 4 级联升级 5　执行板升级
    struct sOtaUpIt usb; // USB升级状态
    struct sOtaUpIt net; // 网络升级状态
    struct sOtaUpIt web; // 网页升级状态
    struct sOtaUpIt slave; // 级联升级状态
    struct sOtaUpIt outlet; // 执行板升级状态
    struct sOtaUpIt rootfs; // 文件系统升级状态
    char host[NAME_SIZE]; // 服务端地址
};

struct sWebCfg{
    ushort http_en;
    ushort http_port;
    ushort http_redirect;

    ushort https_en;
    ushort https_port;
};

/**
 * 数据包
 */
struct sDataPacket
{
    uint mem_size; // 共享内存大小
    struct sWebCfg web; // 网页配置信息
    struct sOtaUpdater ota; // 升级信息
    struct sNetInterface net; //设备IP
    struct sDevData data[DEV_NUM]; //设备数据
    struct sDevLogin login[USER_NUM];
};


enum DType{Tg, Line, Loop, Output, Group, Dual, Env=6, Sensor};
enum DTopic{Relay=1, Vol, Cur, Pow, Ele, PF, ArtPow, ReactivePow, HdaEle, LineVol, Tem=11, Hum, Door1=21, Door2, Water, Smoke, Wind};
enum DSub{Size, Value, Rated, Alarm, VMax, VMin, VCrMin, VCrMax, EnAlarm, DPeak, DStamp, DHda,
          UpDelay=4, ResetDelay, OverrunOff, TimingEn, RelayEn, RelayCnt, Relays=11};
enum DTxType{Tx, TxWeb, TxModbus, TxSnmp, TxRpc, TxJson, TxWebocket, TxSsh};
enum DOtaCode{DOta_ok, DOta_Usb, DOta_Net, DOta_Web, DOta_Slave, DOta_Outlet, DOta_Rootfs};
enum FaultCode{DTC_OK, DTC_VOL=1, DTC_CUR=2, DTC_ELE=4, DTC_POW=8, DTC_CASCADE=16};
enum AlarmCode{Ok, Min=1, CrMin=2, CrMax=4, Max=8};
enum DevMode{DM_Standard, DM_Cascade, DM_Dual, DM_Rtu};

struct sDataItem
{
#ifndef SUPPORT_C
    sDataItem():addr(0),txType(0),rw(0),value(0){}
#endif
    uchar addr; // 地址
    uchar type; // 1 相数据  2 回路数据 ３　输出位数据 4组数据 6 环境 7 传感器
    uchar topic; // 1 开关  2 电压  3 电流  4 功率  11温度 12湿度
    uchar subtopic;  // 0 Size 1 当前值 2 额定值 3 报警状态  11 多开关控制
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar id; // 0 表示统一设置
    uchar rw; // 0 读  1 写
    uint value;
};

enum SFnCode{OutputName=10, Uuts, ECfgNum, EDevInfo, EDevLogin,
             EModbus, ESnmp, ERpc, EPush, EMqtt, EAmqp, EODBC, ERedis=28,
             EOutput=22, EGroup, EDual, EGrouping, EGroupSet,
             EVersion=30, ESercret, ETlsCert, EWhiteList,
             EINet=41, EWeb, ENtp, ESmtp, ESsh, ESysLog, ELogCfg, ERadius, ELdap,
             ELog=81, EHda, EPro=91, EOta, EDgsNet, EDgsDev, EBR, ESys, ECmd=111};

struct sCfgItem {
#ifndef SUPPORT_C
    sCfgItem():addr(0),id(0){}
#endif
    uchar txType; // 通讯类型 1 UDP  3:SNMP  4：Zebra
    uchar addr; // 地址
    uchar type; // 10 输出位  11 UUT信息
    uchar fc; // 功能码　0 表示统一设置
    uchar id;
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
