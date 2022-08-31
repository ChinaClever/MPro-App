#ifndef __FUN_H__
#define __FUN_H__
#include "gshm.h"
#include "sqlite3.h"
#include "inirw.h"
#include "config_shm.h"
#include "gb2312.h"
#include "mongoose.h"

struct serialdata *shm;

//用户组
struct group
{
	 char groupname[32];
	 int outputper[10][42];
	 int setper[10];
}gp[10];

//用户信息
struct userinfo
{
	 char username[32];
	 char psd[32];
	 char mail[3][128]; 
	 int grp;
	 char tel[32];
	 char ip[32];
	 __time_t tm;
}uinfo[10];


struct useronline
{
	long ip;
	char name[32];
	int gpindex;
}onlineper[10];



struct logkwh
{
	int totalkwh[3];
	int kwh[42];
};



int  initweb();
int  userreset();
int  userlogininfo(char *user,char *psd,long ip);

void writenpminfo();
void writegpinfo();
void writeuserinfo();

char *getrequest(char *request,int counter,char *tok);
void req(char * request,char * order,int couter);

int  checkuserper(long ip,int wcone);

char *decodeRequest(char *req);
#endif
