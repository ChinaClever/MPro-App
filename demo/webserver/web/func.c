#include "func.h"


char *getrequest(char *request,int counter,char *tok)
{
	char *rec = NULL;
	int num = 0;
	rec = strtok(request,tok);
	while(rec)
	{
		if(counter = num)
		{
			return rec;
		}
		num++;
	}
	return NULL;
}


struct serialdata *shmdata()
{
	key_t shmkey;
	int shmid;
	shmkey=ftok("/",'b');
	shmid=shmget(shmkey,sizeof(struct serialdata)*2,0666|IPC_CREAT);
	if(shmid==-1)
		return NULL;
	struct serialdata * addr;
		addr=(struct serialdata*)shmat(shmid,0,0);
	if(addr==(struct serialdata*)-1)
		return NULL;
	return addr;
}




//清除登录在线用户信息
void initper()
{
	memset(onlineper,0,sizeof(struct useronline)*10);
}


void emptyuser(char *user,long ip)
{
	int i,j,flag;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ip && ip)
		{
			memset(onlineper[i].name,0,sizeof(char)*32);
			onlineper[i].ip = 0;
			onlineper[i].gpindex=-1;
		}
	}
	for(i=0;i<10;i++)
	{
		if(strcmp(onlineper[i].name,user) == 0 && strlen(user))
		{
			memset(onlineper[i].name,0,sizeof(char)*32);
			onlineper[i].ip = 0;
			onlineper[i].gpindex=-1;
		}
	}
}


//
int userlogininfo(char *user,char *psd,long ip)
{
	int index=0, i=0;
	int j,userindex=0;
	int userflag=0;
	for(i=0;i<10;i++)
	{
		if(strcmp(uinfo[i].username,user)==0 && strlen(user) && strcmp(uinfo[i].psd,psd)==0 && strlen(psd))
		{
			userflag=1;
			userindex = i;
		}
	}
	if(!userflag)
	{
		return -1;
	}
	emptyuser(user,ip);

	for(i=0;i<10;i++)
	{
		if(!strlen(onlineper[i].name))
		{
			index = i;
			break;
		}
	}
	if(index>9)
	{
		index=0;
	}
	onlineper[index].ip = ip;
	memset(onlineper[index].name,0,sizeof(char)*32);
	strcpy(onlineper[index].name,user);
	onlineper[index].gpindex = uinfo[userindex].grp;
	return 0;
}

int checkuserper(long ip,int wcone)
{
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ip && ip)
		{
			break;
		}
	}
	if(i>9)
		return 1;
	if(onlineper[i].gpindex==0)
	{
		return 0;
	}
	if(wcone<0 || wcone>9)
		return 2;
	if(onlineper[i].gpindex<0 || onlineper[i].gpindex>9)
		return 3;
	if(gp[onlineper[i].gpindex].setper[wcone]==0)
		return 4;
	return 0;
}

int userreset()
{
	int i,j;
	FILE *fp=NULL;

	//复位用户组信息
	memset(gp,0,sizeof(struct group)*10);
	strcpy(gp[0].groupname,"admin");
	for(i=0; i<10; i++)
	{
		gp[0].setper[i]=1;
	}
	for(i=0; i<10; i++)
	{
		for(j=0; j<42; j++)
		{
			gp[0].outputper[i][j]=1;
		}
	}
	fp=fopen("/clever/ginfo","w+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);
		fwrite(gp,sizeof(struct group),10,fp);
	}
	fclose(fp);

	//复位用户信息
	fp=NULL;
	memset(uinfo,0,sizeof(struct userinfo)*10);
	fp=fopen("/clever/userinfo","w+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);
		fwrite(uinfo,sizeof(struct userinfo),10,fp);
	}
	fclose(fp);

	shm->userRegisterFlag=0;

	shm->writenpminfo=1;
	sleep(2);

	return 0;
}


void writegpinfo()
{
	FILE *fp=NULL;
	fp=fopen("/clever/ginfo","w+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);//HEAD
		fwrite(gp,sizeof(struct group),10,fp);
	}
	fclose(fp);
}

void writeuserinfo()
{
	FILE *fp=NULL;
	fp=fopen("/clever/userinfo","w+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);//HEAD
		fwrite(uinfo,sizeof(struct userinfo),10,fp);
	}
	fclose(fp);
}

int initweb()
{
	shm = shmdata();
	if(shm==NULL)
	{
		return -1;
	}

	FILE *fp=NULL;
	memset(gp,0,sizeof(struct group)*10);
	fp=fopen("/clever/ginfo","a+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);
		fread(gp,sizeof(struct group),10,fp);
	}
	fclose(fp);

	fp=NULL;
	memset(uinfo,0,sizeof(struct userinfo)*10);
	fp=fopen("/clever/userinfo","a+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);
		fread(uinfo,sizeof(struct userinfo),10,fp);
	}
	fclose(fp);

	initper();

	return 0;
}

void req(char * request,char * order,int couter)
{
	int i=0;
	char *p;
	p = request;
	while((char)(*p)!='\0')
	{
		if((char)(*p)=='=')
		{
			if(i==couter)
			{
				char *buf =p+1;
				int j=0;
				while((char)(*buf)!='\0' && (char)(*buf)!='&')
				{
					j++;
					buf++;
				}
				strncpy(order,p+1,j);
				return;
			}
			i++;
		}
		p++;
	}
}


// 解码url请求字符串 ,分配字符串，取得 html解码，gbk解码后的字符串，在调用处，需释放字符串
// 一个url 只有一个请求
char *decodeRequest(char *req)
{
    if(req)
    {
        int len = strlen(req);
        char *req_dec = malloc(len);
        char *req_utf8;
        url_decode(req, len, req_dec, len, 0);
        Gb2312ToUtf8(req_dec, strlen(req_dec), &req_utf8);
        free(req_dec);
        return req_utf8;
    } 
	return NULL;
}
