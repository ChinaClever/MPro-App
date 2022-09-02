#include "func.h"


#if 0
#define PRINTF
#endif

#if !defined(LISTENING_PORT)
#endif

#define CONF_FILE		"/clever/conf/zebraconf.conf"

enum channel
{
	CH_ZEBRA,
	CH_SNMP,
	CH_TCP,
	CH_ZEBRA_SNMP,
	CH_ZEBRA_TCP
};



static void reply_status(struct mg_connection *conn, int err_code, char *err_string)
{
    mg_printf(conn, "HTTP/1.1 %d %s\r\n\r\n", err_code, err_string);//
    return ;
}

int monitorActivity;
int monitor=1;

void *thread_timer()
{
	int count=0;
	while(1)
	{
		sleep(1);
		if(monitor==1)
		{
			count=0;
			monitorActivity=1;
			continue;
		}	
		else
		{
			count++;
			if(count>=60)//10分钟
			{
				monitorActivity=0;
			}
		}
	}
}

static void monitorMovement(struct mg_connection *conn, const struct mg_request_info *ri) 
{
#ifdef PRINTF
	printf("monitorMovement:%s\n", ri->query_string);
#endif

	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
        free(request);
    }
	monitor=atoi(order1);
	free(order1);


#ifdef PRINTF
	printf("monitorMovement-monitor:%d\t%d\n", monitor,monitorActivity);
#endif

	if(monitorActivity==1)
	{
		reply_status(conn, 200, "OK");
	}
	else
	{
		mg_printf(conn, "HTTP/1.1 302 Found\r\n"
			"Set-Cookie: original_url=%s\r\n"
			"Location: %s\r\n\r\n",
			"/", "/login.html");
	}
}


static void check_register(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char urlList[256];
	memset(urlList,0,sizeof(urlList));
	sprintf(urlList,"%d?%d?check_register?", shm->Language, shm->userRegisterFlag);

	mg_printf(conn, urlList);
	
#ifdef PRINTF
	printf("check_register:%s\n", urlList);
#endif
}


static void user_register(struct mg_connection *conn, const struct mg_request_info *ri)
{
#ifdef PRINTF
	printf("user_register:%s\n", ri->query_string);
#endif

	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(len);
	char *order2 = (char *)malloc(len);
	if(request)
	{
		memset(order1,0,len);
		req(request,order1,0);
		memset(order2,0,len);
		req(request,order2,1);
		free(request);
	}


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

	//注册用户信息
	fp=NULL;
	memset(uinfo,0,sizeof(struct userinfo)*10);
	strcpy(uinfo[0].username,order1);
	strcpy(uinfo[0].psd,order2);
	fp=fopen("/clever/userinfo","w+");
	if(fp)
	{
		fseek(fp,0,SEEK_SET);
		fwrite(uinfo,sizeof(struct userinfo),10,fp);
	}
	fclose(fp);


	shm->userRegisterFlag=1;

	// char cmdStr[256];
	// memset(cmdStr,0,sizeof(cmdStr));
	// sprintf(cmdStr,"/clever/bin/insertdb 32 %s 3 1",order1);	
	// system(cmdStr);
	// memset(cmdStr,0,sizeof(cmdStr));
	// sprintf(cmdStr,"/clever/bin/syslog_send 3 'user register' '%s'",order1);
	// system(cmdStr);

	free(order1);
	free(order2);

	writeuserinfo();
	shm->writenpminfo=1;
	system("sync");

	mg_printf(conn, "HTTP/1.1 200 OK\r\n"
					"Content-Type: text/plain\r\n\r\n"
					"%s", "OK");
}


static void user_login(struct mg_connection *conn, const struct mg_request_info *ri) 
{
#ifdef PRINTF
	printf("user_login:%s\n", ri->query_string);
#endif

	// sleep(3);//放暴力破解
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(len);
	char *order2 = (char *)malloc(len);
	if(request)
	{
		memset(order1,0,len);
		req(request,order1,0);
		memset(order2,0,len);
		req(request,order2,1);
		free(request);
	}

	FILE *file = NULL;
	char line[255];
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '25p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	int rec =-1;
	if(line[0]=='1' || line[0]=='2')
	{
		char rad[255];
		memset(rad,0,sizeof(char)*255);
		sprintf(rad,"/clever/bin/rad %s %s",order1,order2);
		file = NULL;
		file = popen(rad, "r");
		char radrec[32];
		memset(radrec,0,sizeof(char)*32);
		if(file!= NULL)
		{
			if (fgets(radrec, 32, file) == NULL)
			{
				pclose(file);
				return;
			}
		}
		pclose(file);
		printf("login:ri->remote_ip=%ld\n",ri->remote_ip);
		if(radrec[0]=='O' && radrec[1]=='K')
		{
			onlineper[0].ip = ri->remote_ip;
			memset(onlineper[0].name,0,sizeof(char)*32);
			strncpy(onlineper[0].name,order1,sizeof(char)*30);
			onlineper[0].gpindex = 0;
			rec=0;
		}
        else if(strncmp(radrec, "ERR", 3) && line[0] == '2')
        {
            rec = userlogininfo(order1,order2,ri->remote_ip);
        }
	}
	else
	{
		rec = userlogininfo(order1,order2,ri->remote_ip);
	}
	if(rec==-1)
	{
		free(order1);
		free(order2);
		mg_printf(conn, "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain\r\n\r\n"
			"%s", "ERR");
		
		
		return;
	}
	char dbinfo[255];
	memset(dbinfo,0,sizeof(char)*255);
	sprintf(dbinfo,"/clever/bin/insertdb 1 %s 3 1",order1);
	system(dbinfo);
	// char sys_info[255];
	// memset(sys_info,0,sizeof(char)*255);
	// sprintf(sys_info,"/clever/bin/syslog_send 3 'User Login' '%s'",order1);
	// system(sys_info);
	free(order1);
	free(order2);
	//loginFreq=0;
	mg_printf(conn, "HTTP/1.1 302 Found\r\n"
			"Set-Cookie: original_url=%s\r\n"
			"Location: %s\r\n\r\n",
			"/", "/index.html");
}


static void npmslavelist(struct mg_connection *conn, const struct mg_request_info *ri) 
{
	int i;
	char index[32];
	char list[255];
	memset(list,0,sizeof(list));
	
	memset(index,0,sizeof(index));
	sprintf(index,"%d?",shm->Language);
	strcat(list,index);
	
	memset(index,0,sizeof(index));
	sprintf(index,"%d?",shm->type[0]);
	strcat(list,index);

	for(i=0;i<10;i++)
	{
		if(strlen(shm->devicename[i]) && shm->online[i])
		{
			strcat(list,shm->devicename[i]);
			strcat(list,"?");
			memset(index,0,sizeof(index));
			sprintf(index,"%d?",i);
			strcat(list,index);
		}
	}

	mg_printf(conn, list);
}


static void langflag(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
    }
	int index = atoi(order);
	free(order);
	
	shm->Language=index;
	char setbuf[255];
	memset(setbuf,0,sizeof(char)*255);
	sprintf(setbuf,"sed -i '1cLanguage=%d' /clever/conf/language",index);
	system(setbuf);
	system("sync");
	
	reply_status(conn, 200, "OK");
	return;
}


/**home.html**/
static void total(struct mg_connection *conn, const struct mg_request_info *ri) 
{
	//if(checkuserper(ri->remote_ip,1))
	//{
	//	mg_printf(conn, "flase");
	//	return;
	//}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	if(ms<0 || ms>9)
	{
		ms=0;
	}
	free(order);
	
	int i;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		mg_printf(conn, "failed");
		return;
	}
	
	char buf[256];
	char list[1024];
	memset(list,0,sizeof(list));
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%.2f?%.2f?%.1f?%d?%d?",
				(float)(shm->Lcur[ms][0]*shm->Lvol[ms][0]*shm->Lpowerfactor[ms][0]+shm->Lcur[ms][1]*shm->Lvol[ms][1]*shm->Lpowerfactor[ms][1]+shm->Lcur[ms][2]*shm->Lvol[ms][2]*shm->Lpowerfactor[ms][2])/100000000,
				(float)(shm->Lcur[ms][0]*shm->Lvol[ms][0]+shm->Lcur[ms][1]*shm->Lvol[ms][1]+shm->Lcur[ms][2]*shm->Lvol[ms][2])/1000000,
				(float)(shm->Lkwh[ms][0]+shm->Lkwh[ms][1]+shm->Lkwh[ms][2])/10,
				shm->frequency[ms],
				shm->leakageCurrent[ms]);
	strcat(list,buf);
	
	int alarm;
	for(i=0;i<3;i++)
	{
		alarm=0;
		if(shm->Lcur[ms][i]>=shm->LminCcrit[ms][i] && shm->Lcur[ms][i]<=shm->LmaxCcrit[ms][i])
			alarm=0;
		else if(shm->Lcur[ms][i]<shm->Lmincur[ms][i] || shm->Lcur[ms][i]>shm->Lmaxcur[ms][i])
			alarm=2;
		else
			alarm=1;
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%.2f?%.1f?%.2f?",
					alarm,
					(float)shm->Lcur[ms][i]/100,
					(float)shm->Lvol[ms][i]/10,
					(float)shm->Lpowerfactor[ms][i]/100);
		strcat(list,buf);
	}
	
	int a=0;int b=0;
	for(i=0;i<6;i++)
	{
		b=shm->linebit[ms][i]+b;
		a=b-shm->linebit[ms][i]+1;
		
		memset(buf,0,sizeof(buf));
		sprintf(buf,"C%d?%d?%d-%d?%.2fA?%.2fA?%.1fV?%.2fkVA?%.1fkWh?",
					i+1,
					shm->breaker[ms][i],
					a,
					b,
					(float)shm->Ccur[ms][i]/100,
					(float)shm->Cmaxcur[ms][i]/100,
					(float)shm->Cvol[ms][i]/10,
					(float)shm->Ccur[ms][i]*shm->Cvol[ms][i]/1000000,
					(float)shm->Ckwh[ms][i]/10);
		strcat(list,buf);
	}
	
	for(i=0;i<2;i++)
	{
		alarm=0;
		if(shm->temp[ms][i]>=shm->dcritT[ms][i] && shm->temp[ms][i]<=shm->ucritT[ms][i])
			alarm=0;
		else if(shm->temp[ms][i]<shm->minT[ms][i] || shm->temp[ms][i]>shm->maxT[ms][i])
			alarm=2;
		else
			alarm=1;
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%d?",alarm,shm->temp[ms][i]);
		strcat(list,buf);
		
		alarm=0;
		if(shm->hum[ms][i]>=shm->dcritH[ms][i] && shm->hum[ms][i]<=shm->ucritH[ms][i])
			alarm=0;
		else if(shm->hum[ms][i]<shm->minH[ms][i] || shm->hum[ms][i]>shm->maxH[ms][i])
			alarm=2;
		else
			alarm=1;
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%d?",alarm,shm->hum[ms][i]);
		strcat(list,buf);
	}
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?0?%d?%d?",shm->door[ms],shm->water[ms],shm->smoke[ms]);
	strcat(list,buf);
	
	alarm=0;
	
	if(shm->warnflag[ms][0]==1)
	{
		alarm=2;
	}
	else if(shm->alarmflag[ms][0]==1)
	{
		alarm=1;
	}
	
	for(i=0;i<6;i++)
	{
		if(shm->breaker[ms][i]==1)
		{
			alarm=2;
		}
	}
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?%s?%d?%d?%d?%d?%d?%d?",
				alarm,
				shm->deviceaddr[ms],
				shm->runtime[ms],
				shm->type[ms],
				shm->mainVersion,
				shm->boardVersion[0],
				shm->boardVersion[1],
				shm->boardVersion[2]
			);
	strcat(list,buf);

	mg_printf(conn,list);	
}
/**output.html**/
static void getoutput(struct mg_connection *conn, const struct mg_request_info *ri) 
{
	//if(checkuserper(ri->remote_ip,1))
	//{
	//	mg_printf(conn, "flase");
	//	return;
	//}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		mg_printf(conn, "failed");
		return;
	}
	
	char buf[255];
	char list[1024*2];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);

	if(shm->gourpflag[ms]==0)
	{
		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?0?0?0?0?");
					continue;
				}
			}
			
			memset(buf,0,sizeof(buf));
			
			sprintf(buf,"%d?%s?",shm->outputexist[ms][i],shm->outputname[ms][i]);
			strcat(list,buf);
			
			if(shm->type[ms]%4==2 || shm->type[ms]%4==1)
			{
				strcat(list,"--?");
			}
			else
			{
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%d?",shm->swicth[ms][i]);
				strcat(list,buf);
			}	
			
			if(shm->type[ms]%4==3 || shm->type[ms]%4==1)
			{
				strcat(list,"--?--?--?--?--?");
			}
			else
			{
				memset(buf,0,sizeof(buf));
				if(i<shm->linebit[ms][0])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][0]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0] && i<shm->linebit[ms][0]+shm->linebit[ms][1])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][1]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][2]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][3]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3]+shm->linebit[ms][4])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][4]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else 
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][5]/1000000,(float)shm->kwh[ms][i]/10);
				}
				strcat(list,buf);
				
				if((shm->cur[ms][i]>shm->maxcur[ms][i]) || (shm->cur[ms][i]<shm->mincur[ms][i]))
				{
					strcat(list,"2?");
				}
				else if((shm->cur[ms][i]>shm->maxcrit[ms][i]) || (shm->cur[ms][i]<shm->mincrit[ms][i]))
				{
					strcat(list,"1?");
				}
				else
				{
					strcat(list,"0?");
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?0?0?0?0?");
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		int num=1;
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
			
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?0?0?0?0?");
						continue;
					}
				}
				
				memset(buf,0,sizeof(255));
				if(shm->outputexist[ms][i]>0)
				{
					
					sprintf(buf,"%d?%s?",shm->outputexist[ms][i],shm->outputname[ms][i]);
					strcat(list,buf);
					num++;
				}
				else
				{
					strcat(list,"0?0?");
				}

				if(shm->type[ms]%4==2 || shm->type[ms]%4==1)
				{
					strcat(list,"--?");
				}
				else
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?",shm->swicth[ms][i]);
					strcat(list,buf);
				}	

				if(shm->type[ms]%4==3 || shm->type[ms]%4==1)
				{
					strcat(list,"--?--?--?--?");
				}
				else
				{
					memset(buf,0,sizeof(buf));
				if(i<shm->linebit[ms][0])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][0]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0] && i<shm->linebit[ms][0]+shm->linebit[ms][1])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][1]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][2]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][3]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else if(i>=shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3] && i<shm->linebit[ms][0]+shm->linebit[ms][1]+shm->linebit[ms][2]+shm->linebit[ms][3]+shm->linebit[ms][4])
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][4]/1000000,(float)shm->kwh[ms][i]/10);
				}
				else 
				{
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->cur[ms][i]/100,(float)shm->powerfactor[ms][i]/100,(float)shm->cur[ms][i]*shm->Cvol[ms][5]/1000000,(float)shm->kwh[ms][i]/10);
				}
				strcat(list,buf);
				
				if((shm->cur[ms][i]>shm->maxcur[ms][i]) || (shm->cur[ms][i]<shm->mincur[ms][i]))
				{
					strcat(list,"2?");
				}
				else if((shm->cur[ms][i]>shm->maxcrit[ms][i]) || (shm->cur[ms][i]<shm->mincrit[ms][i]))
				{
					strcat(list,"1?");
				}
				else
				{
					strcat(list,"0?");
				}
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			if(flag_2[i]==1 && shm->type[ms]%4!=1)
			{
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%d?%s?",num+i,shm->groupname[ms][i]);
				strcat(list,buf);
				
				if(shm->type[ms]%4==2)
				{
					strcat(list,"--?");
				}
				else
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?",shm->groupswicth[ms][i]);
					strcat(list,buf);
				}	
				
				if(shm->type[ms]%4==3 || shm->type[ms]%4==1)
				{
					strcat(list,"--?--?--?--?");
				}
				else
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%.2f?%.2f?%.2f?%.1f?",(float)shm->gpcur[ms][i]/100,(float)shm->gppowerfactor[ms][i]/100,(float)shm->gpcur[ms][i]*shm->Cvol[ms][0]/1000000,(float)shm->gpkwh[ms][i]/10); 
					strcat(list,buf);
					
					if(shm->warnflag[ms][i+43]==1)
					{
						strcat(list,"2?");
					}
					else if(shm->alarmflag[ms][i+43]==1)
					{
						strcat(list,"1?");
					}
					else
					{
						strcat(list,"0?");
					}
					
					//if((shm->gpcur[ms][i]>shm->gpmaxcrit[ms][i]) || (shm->gpcur[ms][i]<shm->gpmincrit[ms][i]))
					//{
					//	strcat(list,"2?");
					//}
					//else if((shm->gpcur[ms][i]>shm->gpmaxcur[ms][i]) || (shm->gpcur[ms][i]<shm->gpmincur[ms][i]))
					//{
					//	strcat(list,"1?");
					//}
					//else
					//{
					//	strcat(list,"0?");
					//}
				}
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?0?0?");
			}
		}
		
	}

	mg_printf(conn, list);
}


/**user.html**/
static void getuserinfo(struct mg_connection *conn, const struct mg_request_info *ri) 
{
	char list[1024];
	int i;
	memset(list,0,sizeof(list));
	strcat(list,"getuserinfo?");
	for(i=0;i<10;i++)
	{
		if(strlen(uinfo[i].username))
		{
			strcat(list,uinfo[i].username);
			strcat(list,"?");
		}
	}

#ifdef PRINTF
	printf("getuserinfo:%s\n", list);
#endif

	mg_printf(conn, list);
}

static void getgroupinfo(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char list[1024];
	int i;
	memset(list,0,sizeof(char)*1024);
	strcat(list,"getgroupinfo?");
	for(i=0;i<10;i++)
	{
		if(strlen(gp[i].groupname))
		{
			strcat(list,gp[i].groupname);
			strcat(list,"?");
		}
	}

#ifdef PRINTF
	printf("getgroupinfo:%s\n", list);
#endif

	mg_printf(conn, list);
}

static void userlistinfo(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("userlistinfo function----Start------\n");
	char list[1024];
	int i=0;
	memset(list,0,sizeof(char)*1024);
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	printf("request1 :%s\n",request);
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	printf("request2 :%s\n",request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	printf("order:%s\n",order);
	for(i=0;i<10;i++)
	{
		if(strcmp(uinfo[i].username,order)==0 && strlen(order))
		{
			break;
			printf("order:%s\n i:%d\n username:%s\n",order,i,uinfo[i].username);
		}
	}
	if(i>9)
	{	
		FILE *fp=NULL;
		memset(uinfo,0,sizeof(struct userinfo)*10);
		fp=fopen("/clever/userinfo","a+");
		if(fp)
		{
			fseek(fp,0,SEEK_SET);
			fread(uinfo,sizeof(struct userinfo),10,fp);
		}
		fclose(fp);	
		free(order);
		// mg_printf(conn, "");
		strcat(list,uinfo[0].username);
		strcat(list,"?");
		strcat(list,uinfo[0].psd);
		strcat(list,"?");
		int j;
		for(j=0;j<3;j++)
		{
		strcat(list,uinfo[0].mail[j]);
		strcat(list,"?");
		}
		strcat(list,uinfo[0].tel);
		strcat(list,"?");
		strcat(list,gp[uinfo[0].grp].groupname);
		mg_printf(conn, list);
		printf("-----------------!!!!!--------\n");
		return;
	}
	strcat(list,uinfo[i].username);
	strcat(list,"?");
	strcat(list,uinfo[i].psd);
	strcat(list,"?");
	
	int j=0;
	for(j=0;j<3;j++)
	{
		strcat(list,uinfo[i].mail[j]);
		strcat(list,"?");
	}
	strcat(list,uinfo[i].tel);
	strcat(list,"?");
	strcat(list,gp[uinfo[i].grp].groupname);
	free(order);
	printf("userlistinfo function----End------\n");
	mg_printf(conn, list);
}

static void deleteuser(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	int i;
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		 free(request);
	}
	for(i=0;i<10;i++)
	{
		if(strcmp(uinfo[i].username,order)==0 && strlen(order))
		{
			break;
		}
	}
	if(i>9 || i<=0)
	{
		free(order);
		mg_printf(conn, "err");
		return;
	}
	memset(uinfo[i].username,0,sizeof(char)*32);
	memset(uinfo[i].psd,0,sizeof(char)*32);
	
	int j=0;
	for(j=0;j<3;j++)
	{
		memset(uinfo[i].mail[j],0,sizeof(char)*128);
	}

	memset(uinfo[i].tel,0,sizeof(char)*32);
	uinfo[i].grp = 0;

	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 30 %s %s 1",onlineper[i].name,order);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 3 'User Adminstor:' 'User %s was successfully deleted.'", order);
		system(sys_info);
	}
	free(order);
	writeuserinfo();
	system("sync");
	reply_status(conn, 200, "OK");
}

static void adduser(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("adduser function----Start------\n");
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	int i;
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	printf("request1 :%s\n",request);
	request = decodeRequest(request);
	printf("request2 :%s\n",request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	char *order7 = (char *)malloc(sizeof(char)*len);

	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		free(request);
	}
	for(i=0;i<10;i++)
	{
		if(strcmp(uinfo[i].username,order1)==0 && strlen(order1))
		{
			break;
		}
	}
	if((i>=0 && i<10) || strlen(order1)==0 )
	{
		free(order1);
		free(order2);
		free(order3);
		free(order4);
		free(order5);
	
		free(order6);
		free(order7);		
    
		mg_printf(conn, "err1");
		return;
	}
	int j=-1;
	for(i=0;i<10;i++)
	{
		
		if(strcmp(gp[i].groupname,order7)==0 && strlen(order7))
		{
			j=i;
			break;
		}
	}

	if(j>9 || j<0 || strlen(order7)==0 )
	{
		free(order1);
		free(order2);
		free(order3);
		free(order4);
		free(order5);
	
		free(order6);
		free(order7);
		mg_printf(conn, "err2");
		return;
	}
	for(i=0;i<10;i++)
	{
		if(strlen(uinfo[i].username)==0)
		{
			strncpy(uinfo[i].username,order1,sizeof(char)*30);
			strncpy(uinfo[i].psd,order2,sizeof(char)*30);
		
			strncpy(uinfo[i].mail[0],order3,sizeof(char)*126);
			strncpy(uinfo[i].mail[1],order4,sizeof(char)*126);
			strncpy(uinfo[i].mail[2],order5,sizeof(char)*126);			
		
			strncpy(uinfo[i].tel,order6,sizeof(char)*30);

			uinfo[i].grp = j;
			break;
		}
	}
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 31 %s %s 1",onlineper[i].name,order1);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 3 'User Adminstor:' 'User %s be added to the list of users.'", order1);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	free(order7);

	writeuserinfo();
	system("sync");
	printf("adduser function-------End---\n");
	reply_status(conn, 200, "OK");
}

static void changeuser(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	int i,index = -1;
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	char *order7 = (char *)malloc(sizeof(char)*len);
	char *order8 = (char *)malloc(sizeof(char)*len);

	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);

		
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		memset(order8,0,sizeof(char)*len);
		req(request,order8,7);

		free(request);
	}
	for(i=0;i<10;i++)
	{
		
		if(strcmp(uinfo[i].username,order8)==0 && strlen(order8))
		{
			index = i;
			break;
		}
	}

	if(index<0 || i>9 || strlen(order8)==0)
	{
		free(order1);
		free(order2);
		free(order3);
		free(order4);
		free(order5);
		free(order6);
	
		free(order7);
		free(order8);		 

		mg_printf(conn, "err1");
		return;
	}
	int j=-1;
	for(i=0;i<10;i++)
	{
	
		if(strcmp(gp[i].groupname,order7)==0 && strlen(order7))
		{
			j=i;
			break;
		}
	}

	if(j>9 || j<0 || strlen(order7)==0 )
	{
		free(order1);
		free(order2);
		free(order3);
		free(order4);
		free(order5);
		free(order6);
	
		free(order7);
		free(order8);

		mg_printf(conn, "err2");
		return;
	}
	strncpy(uinfo[index].username,order1,sizeof(char)*30);
	strncpy(uinfo[index].psd,order2,sizeof(char)*30);

	strncpy(uinfo[index].mail[0],order3,sizeof(char)*126);
	strncpy(uinfo[index].mail[1],order4,sizeof(char)*126);
	strncpy(uinfo[index].mail[2],order5,sizeof(char)*126);		

	strncpy(uinfo[index].tel,order6,sizeof(char)*30);
	uinfo[index].grp = j;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 32 %s %s 1",onlineper[i].name,order1);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 3 'User Adminstor:' 'User %s the information is successfully changed.'",order1);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);

	free(order7);
	free(order8);

	writeuserinfo();
	system("sync");
	reply_status(conn, 200, "OK");
}

static void gpmainper1(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
	}
    int i=-1;
    for(i=0;i<10;i++)
    {
    	if(strcmp(gp[i].groupname,order)==0 && strlen(order))
    	{
    		break;
    	}
    }
    if(i>9 || i<0 || strlen(order)==0 )
	{
		free(order);
		mg_printf(conn, "err");
		return;
	}
    char per[128];
    memset(per,0,sizeof(char)*128);
    if(i==0)
    {
    	sprintf(per,"%d?%d?%d?%d?",1,1,1,1);
    }
    else
    {
    	sprintf(per,"%d?%d?%d?%d?",gp[i].setper[0],gp[i].setper[1],gp[i].setper[2],gp[i].setper[3]);
    }
	free(order);
	mg_printf(conn, per);
}

static void savegp(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
        free(request);
	}
	int i=-1;
	for(i=0;i<10;i++)
	{
		if(strcmp(gp[i].groupname,order1)==0 && strlen(order1))
		{
			break;
		}
	}
	if(strlen(order1)==0)
	{
		free(order1);
		free(order2);
		mg_printf(conn, "err");
		return;
	}
	int flags = 0;
	if(i>9)
	{
		flags = 1;
	}
	if(flags)
	{
		for(i=0;i<10;i++)
		{
			if(strlen(gp[i].groupname)==0)
			{
				strncpy(gp[i].groupname,order1,sizeof(char)*30);
				break;
			}
		}
	}
	int j;
	if(i>0 && i<10)
	{
		
		int gpindex=0;
		for(j=0;j<10;j++)
		{
			if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
			{
				gpindex=j;
			}
		}
		if(gpindex>=0 && gpindex<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 33 %s %s 1",onlineper[gpindex].name,order1);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 3 'User group:' 'User group %s the information is successfully changed.'",order1);
			system(sys_info);
		}
		
		for(j=0;j<4;j++)
		{
			if(order2[j]=='1')
			{
				gp[i].setper[j]=1;
			}
			else
			{
				gp[i].setper[j]=0;
			}
		}
	}
	free(order1);
	free(order2);
	writegpinfo();
	system("sync");
	reply_status(conn, 200, "OK");
}

static void deletegp(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
	}
	int i=-1;
	for(i=0;i<10;i++)
	{
		if(strcmp(gp[i].groupname,order)==0 && strlen(order))
		{
			break;
		}
	}
	if(i>9 || i<1 || strlen(order)==0 )
	{
		free(order);
		mg_printf(conn, "err");
		return;
	}
	memset(gp[i].groupname,0,sizeof(char)*32);
	int j,k;
	for(j=0;j<10;j++)
	{
	    	for(k=0;k<24;k++)
		{
    			gp[i].outputper[j][k]=0;
		}
	    	gp[i].setper[j]=0;
	}
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 34 %s %s 1",onlineper[i].name,order);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 3 'User group:' '%s was successfully deleted'",order);
	}
	free(order);
	writegpinfo();
	system("sync");
	reply_status(conn, 200, "OK");
}

static void gpmainper2(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
        free(request);
    }
	int i=-1;
	for(i=0;i<10;i++)
	{
		if(strcmp(gp[i].groupname,order1)==0 && strlen(order1))
		{
			break;
		}
	}
	if(i>9 || strlen(order1)==0 )
	{
		free(order1);
		free(order2);
		mg_printf(conn, "err");
		return;
	}

	int index = atoi(order2);
	if(index>9 || index<0)
	{
		free(order1);
		free(order2);
		mg_printf(conn, "err");
		return;
	}
	char per[128];
	memset(per,0,sizeof(char)*128);
	
	//int L1,L2,L3,L4,L5,L6,L;
	//L1=shm->linebit[0][0];
	//L2=shm->linebit[0][1];
	//L3=shm->linebit[0][2];
	//L4=shm->linebit[0][3];
	//L5=shm->linebit[0][4];
	//L6=shm->linebit[0][5];
	//L=L1+L2+L3+L4+L5+L6;
	
	int j;
	for(j=0;j<42;j++)
	{
		
	//	if(j<(shm->outputnum[index]))
		if(shm->outputexist[0][j])
		{
			if(i)
			{
				if(gp[i].outputper[index][j]==0)
					strcat(per,"0?");
				else
					strcat(per,"1?");
			}
			else
			{
					strcat(per,"1?");
			}
		}
		else
		{
				strcat(per,"2?");
		}
	}
	free(order1);
	free(order2);
	mg_printf(conn, per);
}

static void savegpoutput(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,0))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
    }
	int i=-1;
	for(i=0;i<10;i++)
	{
		if(strcmp(gp[i].groupname,order1)==0 && strlen(order1))
		{
			break;
		}
	}
	int index = atoi(order2);
	if(strlen(order1)==0 || index<0 || index>9)
	{
		free(order1);
		free(order2);
		free(order3);
		mg_printf(conn, "err");
		return;
	}
	int flags = 0;
	if(i>9)
	{
		flags = 1;
	}
	if(flags)
	{
		for(i=0;i<10;i++)
		{
			if(strlen(gp[i].groupname)==0)
			{
				strncpy(gp[i].groupname,order1,sizeof(char)*30);
				break;
			}
		}
	}
	int j;
	if(i>0 && i<10)
	{
		
		int gpindex=0;
		for(j=0;j<10;j++)
		{
			if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
			{
				gpindex=j;
			}
		}
		if(gpindex>=0 && gpindex<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 35 %s %s 1",onlineper[gpindex].name,order1);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 3 'User group:' 'User %s the information is successfully changed.'",onlineper[gpindex].name);
			system(sys_info);
		}
		
		for(j=0;j<42;j++)
		{
			if(order3[j]=='1')
			{
				gp[i].outputper[index][j]=1;
			}
			else
			{
				gp[i].outputper[index][j]=0;
			}
		}
	}
	free(order1);
	free(order2);
	free(order3);
	writegpinfo();
	system("sync");
	reply_status(conn, 200, "OK");
}

/**system.html**/
static void systemset(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
        free(request);
	}
	int set=atoi(order1);
	free(order1);
	if(set==0)
	{
		int i;
		for(i=0;i<10;i++)
		{
			if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
			{
				break;
			}
		}
		if(i>=0 && i<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 36 %s 3 1",onlineper[i].name);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 3 'System command' 'User %s restarted the system'",onlineper[i].name);
			system(sys_info);
		}
		system("sync");
		system("reboot -f");
	}
	else if(set==1)
	{
		int i;
		for(i=0;i<10;i++)
		{
			if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
			{
				break;
			}
		}
		if(i>=0 && i<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 37 %s 3 1",onlineper[i].name);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 3 'System command' 'User %s reset the system'",onlineper[i].name);
			system(sys_info);
		}
		
		FILE *fp=NULL;
		fp = fopen("/clever/conf/eth0-setting","w+");
		if(fp!=NULL)
		{
			fwrite(ipinfo,strlen(ipinfo),1,fp);
			fclose(fp);
		}
		else
		{
			fclose(fp);
		}
		
		fp=NULL;
		fp = fopen("/clever/conf/device","w+");
		if(fp!=NULL)
		{
			fwrite(deviceinfo,strlen(deviceinfo),1,fp);
			fclose(fp);
		}
		else
		{
			fclose(fp);
		}
		
		userreset();	
		
		system("sync");
		system("reboot -f");
	}
	else if(set==2)
	{
		int i;
		for(i=0;i<10;i++)
		{
			if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
			{
				break;
			}
		}
		if(i>=0 && i<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 38 %s 3 1",onlineper[i].name);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 3 'System command' 'User %s exits the system'",onlineper[i].name);
			system(sys_info);
		}
		
		
		for(i=0;i<10;i++)
		{
			if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
			{
				onlineper[i].ip=0;
				memset(onlineper[i].name,0,sizeof(char)*32);
				onlineper[i].gpindex = 0;
			}
		}
		mg_printf(conn, "logout");
		return;
	}
	reply_status(conn, 200, "OK");
}
/**system.html**/
static void getmac(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char buf[255];
	memset(buf,0,sizeof(buf));
	FILE *file = NULL;
	
	file = popen("sed -n '1p' /clever/conf/mac | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(buf, 32, file);
	}
	pclose(file);
	

	mg_printf(conn, "%s?",buf);
}

/**device.html**/
static void getdata(struct mg_connection *conn, const struct mg_request_info *ri)
{
	int ms = shm->ms;
	mg_printf(conn, "%d?%d?%s?%s?%d?%d?%d?",shm->sb, ms, shm->devicename[ms], shm->deviceaddr[ms], shm->lightflag[ms], shm->buzzerflag[ms],shm->loopflag[ms]);
}

/**device.html**/
static void getdevice(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
    }

	int ms = atoi(order);
	free(order);

	mg_printf(conn, "%d?%s?%s?%d?%d?%d?", ms, shm->devicename[ms], shm->deviceaddr[ms], shm->lightflag[ms], shm->buzzerflag[ms],shm->loopflag[ms]);

}

/*device.html*/
static void setmode(struct mg_connection *conn, const struct mg_request_info *ri)
{	
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
        free(request);
    }
	int sb = atoi(order1);
	free(order1);

	shm->sb = sb;
	char setbuf[255];
	memset(setbuf,0,sizeof(char)*255);
	sprintf(setbuf,"sed -i '51crs485=%d' /clever/conf/device",sb);
	system(setbuf);

	shm->writenpminfo=1;
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	
	reply_status(conn, 200, "OK");
	return;
}
/*device.html*/
static void setms(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
    }
	int ms = atoi(order1);

	free(order1);

	int i;
	if(ms>=0 && ms<10)
	{
		shm->ms = ms;
	
		char setbuf[255];
		memset(setbuf,0,sizeof(char)*255);
		sprintf(setbuf,"sed -i '1cms=%d' /clever/conf/device",ms);
		system(setbuf);
		memset(shm->devicename[ms],0,32);
		strcpy(shm->devicename[ms],order2);
		memset(shm->deviceaddr[ms],0,32);
		strcpy(shm->deviceaddr[ms],order3);
		free(order2);
		free(order3);
		shm->writenpminfo=1;
		for(i=0;i<10;i++)
		{
			if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
			{
				break;
			}
		}
		if(i>=0 && i<10)
		{
			char dbinfo[255];
			memset(dbinfo,0,sizeof(char)*255);
			sprintf(dbinfo,"/clever/bin/insertdb 2 %s %d 1",onlineper[i].name,ms);
			system(dbinfo);
			
			char sys_info[255];
			memset(sys_info,0,sizeof(char)*255);
			sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' 'The Master/Slave property was changed.'");
			system(sys_info);
		}
		reply_status(conn, 200, "OK");
		return;
	}
	else
	{
		mg_printf(conn, "err1");
		return;
	}
}

static void setname(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
//	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		// memset(order4,0,sizeof(char)*len);
		// req(request,order4,3);
        free(request);
    }

	int rt = atoi(order1);
	int bz = atoi(order2);
	int bk = atoi(order3);
	// int cmd = atoi(order4);
	free(order1);
	free(order2);
	free(order3);
	// free(order4);
	
	int i;

	
	shm->lightflag[0] = rt;
	shm->buzzerflag[0] = bz;
	shm->loopflag[0] = bk;
	// shm->switchcmd = cmd;
	
	char setbuf[255];
	memset(setbuf,0,sizeof(char)*255);
	sprintf(setbuf,"sed -i '46cbuzzerflag=%d' /clever/conf/device",bz);
	system(setbuf);
	system("sync");
	
	
	
	
	shm->writenpminfo=1;
	
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 3 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' 'The buzzer is set.'");
		system(sys_info);
	}
	
	reply_status(conn, 200, "OK");
	
	return;
	
}

/*group.html*/
static void getgroup(struct mg_connection *conn, const struct mg_request_info *ri)
{	
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);

	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}

	char buf[1024];
	char list[1024*8];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?",shm->gourpflag[ms]);
	strcat(list,buf);
	
	for(i=0;i<42;i++)
	{
		if(onlineper[gpflag].gpindex)
		{
			if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
			{
				strcat(list,"0?0?");
				continue;
			}
		}
		
		
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d?%s?",shm->outputexist[ms][i],shm->outputname[ms][i]);
			strcat(list,buf);
			
			//if(i<shm->outputnum[ms] && (shm->type[ms]%4!=1))
			//{
			//	memset(buf,0,sizeof(buf));
			//	sprintf(buf,"%d?%s?",i+1,shm->outputname[ms][i]);
			//	strcat(list,buf);
			//}
			//else
			//{
			//	memset(buf,0,sizeof(buf));
			//	sprintf(buf,"0?0?");
			//	strcat(list,buf);
			//}
		
	}
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<42;j++)
		{
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d?",shm->group[ms][i][j]);
			strcat(list,buf);
		}
	}

	mg_printf(conn,list);
}

static void setgroup(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	char *order7 = (char *)malloc(sizeof(char)*len);
	char *order8 = (char *)malloc(sizeof(char)*len);
	char *order9 = (char *)malloc(sizeof(char)*len);
	char *order10 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		memset(order8,0,sizeof(char)*len);
		req(request,order8,7);
		memset(order9,0,sizeof(char)*len);
		req(request,order9,8);
		memset(order10,0,sizeof(char)*len);
		req(request,order10,9);
        free(request);
    }
	
	int i, j;
	int ms = atoi(order1);
	int flag=shm->gourpflag[ms]+2;
	shm->gourpflag[ms] = atoi(order2);
	if(flag!=shm->gourpflag[ms])
	{
		for(i=0;i<50;i++)
		{
			shm->ping[ms][i]=0;
			shm->down[ms][i]=0;
			shm->selecttime[ms][i]=0;
		}
	}

	for(j=0;j<42;j++)
	{
		if(order3[j]=='1')
		{
			shm->group[ms][0][j]=1;
		}
		else
		{
			shm->group[ms][0][j]=0;
		}
		
		if(order4[j]=='1')
		{
			shm->group[ms][1][j]=1;
		}
		else
		{
			shm->group[ms][1][j]=0;
		}
		
		if(order5[j]=='1')
		{
			shm->group[ms][2][j]=1;
		}
		else
		{
			shm->group[ms][2][j]=0;
		}
		
		if(order6[j]=='1')
		{
			shm->group[ms][3][j]=1;
		}
		else
		{
			shm->group[ms][3][j]=0;
		}
		
		if(order7[j]=='1')
		{
			shm->group[ms][4][j]=1;
		}
		else
		{
			shm->group[ms][4][j]=0;
		}
		
		if(order8[j]=='1')
		{
			shm->group[ms][5][j]=1;
		}
		else
		{
			shm->group[ms][5][j]=0;
		}
		
		if(order9[j]=='1')
		{
			shm->group[ms][6][j]=1;
		}
		else
		{
			shm->group[ms][6][j]=0;
		}
		
		if(order10[j]=='1')
		{
			shm->group[ms][7][j]=1;
		}
		else
		{
			shm->group[ms][7][j]=0;
		}
	}
	
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	free(order7);
	free(order8);
	free(order9);
	free(order10);
	
	
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{
		shm->slavesetflag[ms][2]=1;
	}
	
	//if(shm->gourpflag[ms]==1) 	//写在writenpminfo()后,因为不需要保存
	//{
	//	shm->gourpstate[ms] = 1;
	//}
	
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 4 %s '%s' 1",onlineper[i].name,shm->devicename[ms]);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Group Settings:' '%s grouping.'",shm->devicename[ms]);
		system(sys_info);
	}
	reply_status(conn, 200, "OK");
	return;
}

static void getthreshold(struct mg_connection *conn, const struct mg_request_info *ri)
{
	
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}
	
	char buf[255];
	char list[1024*8];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	
	if(shm->gourpflag[ms]==0)
	{

		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?0?0?0?0?0?");
					continue;
				}
			}
			
			memset(buf,0,sizeof(buf));
			if(shm->type[ms]%4==0)
			{
				sprintf(buf,"%d?%s?%.2f?%d?%.2f?%.2f?%.2f?%.2f?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->cur[ms][i]/100,shm->outputdelay[ms][i],(float)shm->mincur[ms][i]/100,(float)shm->mincrit[ms][i]/100,(float)shm->maxcrit[ms][i]/100,(float)shm->maxcur[ms][i]/100,shm->thresholdflag[ms][i]);
			}
			else if(shm->type[ms]%4==1)
			{
				sprintf(buf,"%d?%s?--?--?--?--?--?--?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->thresholdflag[ms][i]);
			}
			else if(shm->type[ms]%4==2)
			{
				sprintf(buf,"%d?%s?%.2f?--?%.2f?%.2f?%.2f?%.2f?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->cur[ms][i]/100,(float)shm->mincur[ms][i]/100,(float)shm->mincrit[ms][i]/100,(float)shm->maxcrit[ms][i]/100,(float)shm->maxcur[ms][i]/100,shm->thresholdflag[ms][i]);
			}
			if(shm->type[ms]%4==3)
			{
				sprintf(buf,"%d?%s?--?%d?--?--?--?--?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->outputdelay[ms][i],shm->thresholdflag[ms][i]);
			}
			strcat(list,buf);
			
		}
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?0?0?0?0?0?");
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
	//	printf("2---shm->gourpflag[%d]=%d\n",ms,shm->gourpflag[ms]);
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		int num=1;
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
			
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?0?0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?0?0?0?0?0?");
						continue;
					}
				}
				if(shm->outputexist[ms][i]>0)
				{
					memset(buf,0,sizeof(buf));
					if(shm->type[ms]%4==0)
					{
						sprintf(buf,"%d?%s?%.2f?%d?%.2f?%.2f?%.2f?%.2f?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->cur[ms][i]/100,shm->outputdelay[ms][i],(float)shm->mincur[ms][i]/100,(float)shm->mincrit[ms][i]/100,(float)shm->maxcrit[ms][i]/100,(float)shm->maxcur[ms][i]/100,shm->thresholdflag[ms][i]);
					}
					else if(shm->type[ms]%4==1)
					{
						sprintf(buf,"%d?%s?--?--?--?--?--?--?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->thresholdflag[ms][i]);
					}
					else if(shm->type[ms]%4==2)
					{
						sprintf(buf,"%d?%s?%.2f?--?%.2f?%.2f?%.2f?%.2f?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->cur[ms][i]/100,(float)shm->mincur[ms][i]/100,(float)shm->mincrit[ms][i]/100,(float)shm->maxcrit[ms][i]/100,(float)shm->maxcur[ms][i]/100,shm->thresholdflag[ms][i]);
					}
					if(shm->type[ms]%4==3)
					{
						sprintf(buf,"%d?%s?--?%d?--?--?--?--?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->outputdelay[ms][i],shm->thresholdflag[ms][i]);
					}
					strcat(list,buf);
					
					num++;
				}
				else
				{
					strcat(list,"0?0?0?0?0?0?0?0?0?");
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			memset(buf,0,sizeof(char)*255);
			if(flag_2[i]==1)
			{
				memset(buf,0,sizeof(buf));
				if(shm->type[ms]%4==0)
				{
					sprintf(buf,"%d?%s?%.2f?%d?%.2f?%.2f?%.2f?%.2f?%d?",num,shm->groupname[ms][i],(float)shm->gpcur[ms][i]/100,shm->groupdelay[ms][i],(float)shm->gpmincur[ms][i]/100,(float)shm->gpmincrit[ms][i]/100,(float)shm->gpmaxcrit[ms][i]/100,(float)shm->gpmaxcur[ms][i]/100,shm->thresholdflag[ms][i]);
				}
				else if(shm->type[ms]%4==1)
				{
					sprintf(buf,"%d?%s?--?--?--?--?--?--?%d?",num,shm->groupname[ms][i],shm->thresholdflag[ms][i]);
				}
				else if(shm->type[ms]%4==2)
				{
					sprintf(buf,"%d?%s?%.2f?--?%.2f?%.2f?%.2f?%.2f?%d?",num,shm->groupname[ms][i],(float)shm->gpcur[ms][i]/10,(float)shm->gpmincur[ms][i]/100,(float)shm->gpmincrit[ms][i]/100,(float)shm->gpmaxcrit[ms][i]/100,(float)shm->gpmaxcur[ms][i]/100,shm->thresholdflag[ms][i]);
				}
				if(shm->type[ms]%4==3)
				{
					sprintf(buf,"%d?%s?--?%d?--?--?--?--?%d?",num,shm->groupname[ms][i],shm->groupdelay[ms][i],shm->thresholdflag[ms][i]);
				}
				strcat(list,buf);
				
				num++;
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?0?0?0?");
			}
		}
	}

	mg_printf(conn, list);
}

static void setthreshold(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	
	int i,j;
	char str[8];
	req(request,str,0);
	int ms = atoi(str);
	
	char order[50][64];
	memset(order,0,sizeof(order));
	char n1[8],n2[32],n3[8],n4[8],n5[8],n6[8],n7[8],n8[8];
	for(i=0;i<50;i++)
	{
		req(request,order[i],i+1);
		memset(n1,0,sizeof(n1));
		memset(n2,0,sizeof(n2));
		memset(n3,0,sizeof(n3));
		memset(n4,0,sizeof(n4));
		memset(n5,0,sizeof(n5));
		memset(n6,0,sizeof(n6));
		memset(n7,0,sizeof(n7));
		memset(n8,0,sizeof(n8));
		sscanf(order[i],"%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]$%[^$]",n1,n2,n3,n4,n5,n6,n7,n8);			

		
		shm->thresholdflag[ms][i]=atoi(n8);
		if(shm->thresholdflag[ms][i]==1)
		{
			//if(ms==0)
			//{
			//	shm->thresholdflag[ms][i]=0;
			//}
		
			if(atoi(n1)>0 && atoi(n1)<43)
				strcpy(shm->outputname[ms][i],n2);
			if(atoi(n1)>42 && atoi(n1)<51)
				strcpy(shm->groupname[ms][i-42],n2);
			
			if(shm->type[ms]%4==3 || shm->type[ms]%4==0)
			{
				if(atoi(n1)>0 && atoi(n1)<43)
					shm->outputdelay[ms][i] = atoi(n3);
				if(atoi(n1)>42 && atoi(n1)<51)
					shm->groupdelay[ms][i-42] = atoi(n3);
			}
			
			if(shm->type[ms]%4==2 || shm->type[ms]%4==0)
			{
				if(atoi(n1)>0 && atoi(n1)<43)
				{
					shm->mincur[ms][i] = atoi(n4);
					shm->mincrit[ms][i] = atoi(n5);
					shm->maxcrit[ms][i] = atoi(n6);
					shm->maxcur[ms][i] = atoi(n7);
				}
				if(atoi(n1)>42 && atoi(n1)<51)
				{
					shm->gpmincur[ms][i-42] = atoi(n4);
					shm->gpmincrit[ms][i-42] = atoi(n5);
					shm->gpmaxcrit[ms][i-42] = atoi(n6);
					shm->gpmaxcur[ms][i-42] = atoi(n7);
				}
			}
			
			//写日志
			for(j=0;j<10;j++)
			{
				if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
				{
					break;
				}
			}
			if(j>=0 && j<10)
			{
				char dbinfo[255];
				memset(dbinfo,0,sizeof(char)*255);
				char dt[255];
				memset(dt,0,sizeof(char)*255);
				if(i<42)
					sprintf(dt,"%s->%s",shm->devicename[ms],shm->outputname[ms][i]);
				else if(i>41 && i<50)
					sprintf(dt,"%s->%s",shm->devicename[ms],shm->groupname[ms][i-42]);
				sprintf(dbinfo,"/clever/bin/insertdb 5 %s '%s' 1",onlineper[j].name,dt);
				system(dbinfo);
				
				char sys_info[255];
				memset(sys_info,0,sizeof(char)*255);
				sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' '%s(Device name->Output name)information was successfully modified.'",dt);
				system(sys_info);
			}
		}
		
	}
	
	if(shm->type[0]%4==3 || shm->type[0]%4==0)
	{
		shm->delayflag[0]=1;
	}
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{
		shm->slavesetflag[ms][3]=1;
	}
	
	reply_status(conn, 200, "OK");
	return;
}

static void getoverload(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}

	char buf[255];
	char list[1024*2];
	memset(list,0,sizeof(list));

	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?",shm->downflag[ms]);
	strcat(list,buf);
	
	if(shm->gourpflag[ms]==0)
	{
		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?0?0?");
					continue;
				}
			}
			if(shm->type[ms]%4==0)
			{
				
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?%s?%.2f?%.2f?%.2f?%d?"
							,shm->outputexist[ms][i]
							,shm->outputname[ms][i]
							,(float)shm->cur[ms][i]/100
							,(float)shm->mincur[ms][i]/100
							,(float)shm->maxcur[ms][i]/100
							,shm->down[ms][i]);
					strcat(list,buf);	
			
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?");
			}
		}
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?0?0?");
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		int num=1;
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
			
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?0?0?");
						continue;
					}
				}
				if(shm->type[ms]%4==0)
				{
					if(shm->outputexist[ms][i]>0)
					{
						memset(buf,0,sizeof(buf));
						sprintf(buf,"%d?%s?%.2f?%.2f?%.2f?%d?"
							,shm->outputexist[ms][i]
							,shm->outputname[ms][i]
							,(float)shm->cur[ms][i]/100
							,(float)shm->mincur[ms][i]/100
							,(float)shm->maxcur[ms][i]/100
							,shm->down[ms][i]);
						strcat(list,buf);	
						
						num++;
					}
					else
					{
						strcat(list,"0?0?0?0?0?0?");
					}
				}
				else
				{
					strcat(list,"0?0?0?0?0?0?");
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			memset(buf,0,sizeof(buf));
			if(flag_2[i]==1)
			{
				memset(buf,0,sizeof(char)*255);
				sprintf(buf,"%d?%s?%.2f?%.2f?%.2f?%d?"
						,num
						,shm->groupname[ms][i]
						,(float)shm->gpcur[ms][i]/100
						,(float)shm->gpmincur[ms][i]/100
						,(float)shm->gpmaxcur[ms][i]/100
						,shm->down[ms][i+42]);
				strcat(list,buf);
				
				num++;
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?");
			}
		}
	}

	mg_printf(conn, list);
}

static void setoverload(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
    }
	
	int i, j;
	int ms = atoi(order1);
	shm->downflag[ms] = atoi(order2);
	
	for(i=0;i<50;i++)
	{
		if(order3[i]=='1')
		{
			shm->down[ms][i]=1;
		}
		else
		{
			shm->down[ms][i]=0;
		}
	}
	
	free(order1);
	free(order2);
	free(order3);
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{
		shm->slavesetflag[ms][5]=1;
	}
	
	for(j=0;j<10;j++)
	{
		if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(j>=0 && j<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		char dt[255];
		memset(dt,0,sizeof(char)*255);
		sprintf(dt,"%s",shm->devicename[ms]);
		sprintf(dbinfo,"/clever/bin/insertdb 8 %s '%s' 1",onlineper[j].name,dt);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' '%s overrun power setting.'",dt);
		system(sys_info);
	}
	
	
	reply_status(conn, 200, "OK");
	return;
}

static void getping(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}
	
	
	char buf[255];
	char list[1024*2];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?%d?%d?",shm->type[ms],shm->pingflag[ms],shm->pingtime[ms]);
	strcat(list,buf);
	
	char pingaddr[32];
	if(shm->gourpflag[ms]==0)
	{
		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?0?");
					continue;
				}
			}
			
			memset(pingaddr,0,sizeof(pingaddr));
			sprintf(pingaddr,"%d.%d.%d.%d",shm->pingaddr[ms][i][0],shm->pingaddr[ms][i][1],shm->pingaddr[ms][i][2],shm->pingaddr[ms][i][3]);
			
			memset(buf,0,sizeof(buf));
			sprintf(buf,"%d?%s?%s?%d?%d?"
					,shm->outputexist[ms][i]
					,shm->outputname[ms][i]
					,pingaddr
					,shm->pingaction[ms][i]
					,shm->ping[ms][i]);
			strcat(list,buf);	
			
		}
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?0?");
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		int num=1;
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
	
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?0?");
						continue;
					}
				}
				
				memset(pingaddr,0,sizeof(pingaddr));
				sprintf(pingaddr,"%d.%d.%d.%d",shm->pingaddr[ms][i][0],shm->pingaddr[ms][i][1],shm->pingaddr[ms][i][2],shm->pingaddr[ms][i][3]);
				if(shm->outputexist[ms][i]>0)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?%s?%s?%d?%d?"
							,shm->outputexist[ms][i]
							,shm->outputname[ms][i]
							,pingaddr
							,shm->pingaction[ms][i]
							,shm->ping[ms][i]);
					strcat(list,buf);
					
					num++;
				}
				else
				{
					strcat(list,"0?0?0?0?0?");
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			memset(pingaddr,0,sizeof(pingaddr));
			sprintf(pingaddr,"%d.%d.%d.%d",shm->pingaddr[ms][i+42][0],shm->pingaddr[ms][i+42][1],shm->pingaddr[ms][i+42][2],shm->pingaddr[ms][i+42][3]);
			if(flag_2[i]==1)
			{
				memset(buf,0,sizeof(char)*255);
				sprintf(buf,"%d?%s?%s?%d?%d?"
						,num
						,shm->groupname[ms][i]
						,pingaddr
						,shm->pingaction[ms][i+42]
						,shm->ping[ms][i+42]);
				strcat(list,buf);
				
				num++;
			}
			else
			{
				strcat(list,"0?0?0?0?0?");
			}
		}
	}

	mg_printf(conn, list);
}

static void setping(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	
	int i;
	char str[3][8];
	req(request,str[0],0);
	req(request,str[1],1);
	req(request,str[2],2);
	int ms = atoi(str[0]);
	shm->pingflag[ms] = atoi(str[1]);
	shm->pingtime[ms] = atoi(str[2]);
	

	
	
	char order[50][64]={0};
	for(i=0;i<50;i++)
	{
		req(request,order[i],i+3);
		char n1[8],n2[40],n3[8],n4[8];
		sscanf(order[i],"%[^$]$%[^$]$%[^$]$%[^$]",n1,n2,n3,n4);
		if(atoi(n1)>0)
		{
		//	strncpy(shm->pingaddr[0][i],n2,40);
			sscanf(n2,"%d.%d.%d.%d",&shm->pingaddr[ms][i][0],&shm->pingaddr[ms][i][1],&shm->pingaddr[ms][i][2],&shm->pingaddr[ms][i][3]);
			shm->pingaction[ms][i] = atoi(n3);
			shm->ping[ms][i] = atoi(n4);	
		}
	}
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{
		shm->slavesetflag[ms][7]=1;
	}

	int j;
	for(j=0;j<10;j++)
	{
		if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(j>=0 && j<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		char dt[255];
		memset(dt,0,sizeof(char)*255);
		sprintf(dt,"%s",shm->devicename[ms]);
		sprintf(dbinfo,"/clever/bin/insertdb 10 %s '%s' 1",onlineper[j].name,dt);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' '%s ping setting.'",dt);
		system(sys_info);
	}

	reply_status(conn, 200, "OK");
	return;
}

static void gettiming(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
		free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}
	
	
	char buf[255];
	char list[1024*5];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?%d?",shm->type[ms],shm->timeflag[ms]);
	strcat(list,buf);
	
	int a,b,c,d;
	int A,B,C,D;
	int x,y,z;
	int X,Y,Z;
	char ontime[20];
	char offtime[20];
	if(shm->gourpflag[ms]==0)
	{
		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?0?0?");
					continue;
				}
			}
			
			d=shm->uptime[ms][i][0]/100;
			a=shm->uptime[ms][i][0]/10000;
			b=d-a*100;
			c=shm->uptime[ms][i][0]-d*100;
			x=shm->uptime[ms][i][1]/100;
			y=shm->uptime[ms][i][1]-x*100;
			
			D=shm->downtime[ms][i][0]/100;
			A=shm->downtime[ms][i][0]/10000;
			B=D-A*100;
			C=shm->downtime[ms][i][0]-D*100;
			X=shm->downtime[ms][i][1]/100;
			Y=shm->downtime[ms][i][1]-X*100;
			
			if(a==0 && b==0 && c==0 && x==0 && y==0)
			{
				memset(ontime, 0, sizeof(ontime));
				sprintf(ontime, "1970-01-01 00:00");
			}
			else
			{
				memset(ontime, 0, sizeof(ontime));
				sprintf(ontime, "%d-%02d-%02d %02d:%02d", a, b, c, x, y);
			}
			
			if(A==0 && B==0 && C==0 && X==0 && Y==0)
			{
				memset(offtime, 0, sizeof(offtime));
				sprintf(offtime, "1970-01-01 00:00");
			}
			else
			{
				memset(offtime, 0, sizeof(offtime));
				sprintf(offtime, "%d-%02d-%02d %02d:%02d", A, B, C, X, Y);
			}
			
			if(shm->outputexist[ms][i]>0)
			{
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%d?%s?%s?%s?%d?%d?"
						,shm->outputexist[ms][i]
						,shm->outputname[ms][i]
						,ontime
						,offtime
						,shm->timecycle[ms][i]
						,shm->selecttime[ms][i]);
				strcat(list,buf);	
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?");
			}
		}
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?0?0?");
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		int num=1;
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
	
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?0?0?");
						continue;
					}
				}
				
				
				d=shm->uptime[ms][i][0]/100;
				a=shm->uptime[ms][i][0]/10000;
				b=d-a*100;
				c=shm->uptime[ms][i][0]-d*100;
				x=shm->uptime[ms][i][1]/100;
				y=shm->uptime[ms][i][1]-x*100;
				
				D=shm->downtime[ms][i][0]/100;
				A=shm->downtime[ms][i][0]/10000;
				B=D-A*100;
				C=shm->downtime[ms][i][0]-D*100;
				X=shm->downtime[ms][i][1]/100;
				Y=shm->downtime[ms][i][1]-X*100;
				
				if(a==0 && b==0 && c==0 && x==0 && y==0)
				{
					memset(ontime, 0, sizeof(ontime));
					sprintf(ontime, "1970-01-01 00:00");
				}
				else
				{
					memset(ontime, 0, sizeof(ontime));
					sprintf(ontime, "%d-%02d-%02d %02d:%02d", a, b, c, x, y);
				}
				
				if(A==0 && B==0 && C==0 && X==0 && Y==0)
				{
					memset(offtime, 0, sizeof(offtime));
					sprintf(offtime, "1970-01-01 00:00");
				}
				else
				{
					memset(offtime, 0, sizeof(offtime));
					sprintf(offtime, "%d-%02d-%02d %02d:%02d", A, B, C, X, Y);
				}
				
				if(shm->outputexist[ms][i]>0)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?%s?%s?%s?%d?%d?"
						,shm->outputexist[ms][i]
						,shm->outputname[ms][i]
						,ontime
						,offtime
						,shm->timecycle[ms][i]
						,shm->selecttime[ms][i]);
				strcat(list,buf);
					
					num++;
				}
				else
				{
					strcat(list,"0?0?0?0?0?0?");
				}
			}
		}
		
		for(i=0;i<8;i++)
		{
			d=shm->uptime[ms][42+i][0]/100;
			a=shm->uptime[ms][42+i][0]/10000;
			b=d-a*100;
			c=shm->uptime[ms][42+i][0]-d*100;
			x=shm->uptime[ms][42+i][1]/100;
			y=shm->uptime[ms][42+i][1]-x*100;
			
			D=shm->downtime[ms][42+i][0]/100;
			A=shm->downtime[ms][42+i][0]/10000;
			B=D-A*100;
			C=shm->downtime[ms][42+i][0]-D*100;
			X=shm->downtime[ms][42+i][1]/100;
			Y=shm->downtime[ms][42+i][1]-X*100;
			
			if(a==0 && b==0 && c==0 && x==0 && y==0)
			{
				memset(ontime, 0, sizeof(ontime));
				sprintf(ontime, "1970-01-01 00:00");
			}
			else
			{
				memset(ontime, 0, sizeof(ontime));
				sprintf(ontime, "%d-%02d-%02d %02d:%02d", a, b, c, x, y);
			}
			
			if(A==0 && B==0 && C==0 && X==0 && Y==0)
			{
				memset(offtime, 0, sizeof(offtime));
				sprintf(offtime, "1970-01-01 00:00");
			}
			else
			{
				memset(offtime, 0, sizeof(offtime));
				sprintf(offtime, "%d-%02d-%02d %02d:%02d", A, B, C, X, Y);
			}
			
			memset(buf,0,sizeof(char)*255);
			if(flag_2[i]==1)
			{
				memset(buf,0,sizeof(char)*255);
				sprintf(buf,"%d?%s?%s?%s?%d?%d?"
						,num
						,shm->groupname[ms][i]
						,ontime
						,offtime
						,shm->timecycle[ms][42+i]
						,shm->selecttime[ms][42+i]);
				strcat(list,buf);
				
				num++;
			}
			else
			{
				strcat(list,"0?0?0?0?0?0?");
			}
		}
	}
	mg_printf(conn, list);
}

int tiemcom(char order[16], int index, int i, int flag)
{
	int n1,n2,n3,n4,n5;
	if(5 == sscanf(order,"%d-%d-%d %d:%d",&n1,&n2,&n3,&n4,&n5))
	{
		if(n2>=1 && n2<=12 && n3>=1 && n3<=31 && n4>=0 && n4<=23 && n5>=0 && n5<=59)
		{
			if(flag==0)
			{
				shm->uptime[index][i][0]=n1*10000+n2*100+n3;
				shm->uptime[index][i][1]=n4*100+n5;
			}
			else
			{
				shm->downtime[index][i][0]=n1*10000+n2*100+n3;
				shm->downtime[index][i][1]=n4*100+n5;
			}
			
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if(strlen(order)==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static void settiming(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);

	int i;
	char str[2][8];
	req(request,str[0],0);
	req(request,str[1],1);
	int ms = atoi(str[0]);
	shm->timeflag[ms] = atoi(str[1]);
	
	int n=0, m=1;
	int ret1, ret2;
	char order[50][64]={0};
	char n1[8],n2[32],n3[32],n4[8],n5[8];
	for(i=0;i<50;i++)
	{
		req(request,order[i],i+2);
		memset(n1,0,sizeof(n1));
		memset(n2,0,sizeof(n2));
		memset(n3,0,sizeof(n3));
		memset(n4,0,sizeof(n4));
		memset(n5,0,sizeof(n5));
		sscanf(order[i],"%[^$]$%[^$]$%[^$]$%[^$]$%[^$]",n1,n2,n3,n4,n5);	

		if(atoi(n1)>0)
		{
			shm->timecycle[ms][i] = atoi(n4);
			shm->selecttime[ms][i] = atoi(n5);		
			
			ret1=tiemcom(n2, ms, i, n);
			ret2=tiemcom(n3, ms, i, m);
		}
	
		if(ret1==1 || ret2==1)
		{
			mg_printf(conn, "err1");
			return;
		}
	}
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{

		shm->slavesetflag[ms][6]=1;
	}

	int j;
	for(j=0;j<10;j++)
	{
		if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(j>=0 && j<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		char dt[255];
		memset(dt,0,sizeof(char)*255);
		sprintf(dt,"%s",shm->devicename[ms]);
		sprintf(dbinfo,"/clever/bin/insertdb 9 %s '%s' 1",onlineper[j].name,dt);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' '%s timing switch setting.'",dt);
		system(sys_info);
	}

	reply_status(conn, 200, "OK");
	return;
}

static void getsensor(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
    }
	int ms = atoi(order);
	free(order);

	int i;
	char buf[255];
	char list[1024*6];
	memset(list,0,sizeof(char)*1024*6);
	memset(buf,0,sizeof(char)*255);
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	for(i=0;i<2;i++)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%d?%d?%d?%d?%d?%d?"
				,i+1
				,shm->temp[ms][i]
				,shm->minT[ms][i]
				,shm->dcritT[ms][i]
				,shm->ucritT[ms][i]
				,shm->maxT[ms][i]
				,shm->sensorflag[ms][i]
				);
		strcat(list,buf);
	}
	for(i=0;i<2;i++)
	{
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%d?%d?%d?%d?%d?%d?"
				,i+3
				,shm->hum[ms][i]
				,shm->minH[ms][i]
				,shm->dcritH[ms][i]
				,shm->ucritH[ms][i]
				,shm->maxH[ms][i]
				,shm->sensorflag[ms][i+2]
				);
		strcat(list,buf);
	}
	int num=0;
	int max=0;
	if(shm->type[ms]<5)
		max=2;
	else if(shm->type[ms]>4 && shm->type[ms]<9)
		max=3;
	else if(shm->type[ms]>8 && shm->type[ms]<13)
		max=4;
	else if(shm->type[ms]>12 && shm->type[ms]<17)
		max=6;
	else if(shm->type[ms]>16 && shm->type[ms]<21)
		max=1;

	int number=0;
	for(i=0;i<6;i++)
	{
		number=0;
		if(i<max)
		{
			number = num+1;
			++num;
		}
		
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%.2f?%.2f?%.2f?%.2f?%.2f?%d?"
				,number
				,(float)shm->Ccur[ms][i]/100
				,(float)shm->Cmincur[ms][i]/100
				,(float)shm->Cmincrit[ms][i]/100
				,(float)shm->Cmaxcrit[ms][i]/100
				,(float)shm->Cmaxcur[ms][i]/100
				,shm->sensorflag[ms][i+4]
				);
		strcat(list,buf);
	}
	
	for(i=0;i<3;i++)
	{
		number=0;
		if((max==2 || max==4 || max==1) && i==0)	//总电流
		{
			number = num+1;
			++num;
		}
		else if(max==3 || max==6)
		{
			number = num+1;
			++num;
		}
		
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%.2f?%.2f?%.2f?%.2f?%.2f?%d?"
				,number
				,(float)shm->Lcur[ms][i]/100
				,(float)shm->Lmincur[ms][i]/100
				,(float)shm->LminCcrit[ms][i]/100
				,(float)shm->LmaxCcrit[ms][i]/100
				,(float)shm->Lmaxcur[ms][i]/100
				,shm->sensorflag[ms][i+10]
				);
		strcat(list,buf);
	}
	
	for(i=0;i<3;i++)
	{
		number=0;
		if((max==2 || max==4 || max==1) && i==0)	//总电流
		{
			number = num+1;
			++num;
		}
		else if(max==3 || max==6)
		{
			number = num+1;
			++num;
		}
		
		memset(buf,0,sizeof(buf));
		sprintf(buf,"%d?%.1f?%.1f?%.1f?%.1f?%.1f?%d?"
				,number
				,(float)shm->Lvol[ms][i]/10
				,(float)shm->Lminvol[ms][i]/10
				,(float)shm->LminVcrit[ms][i]/10
				,(float)shm->LmaxVcrit[ms][i]/10
				,(float)shm->Lmaxvol[ms][i]/10
				,shm->sensorflag[ms][i+13]
				);
		strcat(list,buf);
	}

	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?%d?%d?"
			,shm->leakageFlag[ms]
			,shm->leakageCurrent[ms]
			,shm->leakageCurrentThreshold[ms]
			);
	strcat(list,buf);
	

	mg_printf(conn, list);
}

static void setsensor(struct mg_connection *conn, const struct mg_request_info *ri)
{

	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	
	int i,j;
	char str[8];
	req(request,str,0);
	int ms = atoi(str);

	req(request,str,17);
	shm->leakageCurrentThreshold[ms]= atoi(str);
	
	// if(i==16)
	// {
	// 	req(request,order[i],i+1);
	// 	shm->leakageCurrentThreshold[ms]=atoi(order[i]);
	// 	break;
	// }

	char order[16][64]={0};
	for(i=0;i<16;i++)
	{
		req(request,order[i],i+1);
		char n1[8],n2[8],n3[8],n4[8],n5[8]={0};
		sscanf(order[i],"%[^$]$%[^$]$%[^$]$%[^$]$%[^$]",n1,n2,n3,n4,n5);
		
		if(shm->type[ms]<5 && (i==6 || i==7 || i==8 || i==9 || i==11 || i==12 || i==14 || i==15))
			continue;
		else if((shm->type[ms]>4 && shm->type[ms]<9) && (i==7 || i==8 || i==9))
			continue;
		else if((shm->type[ms]>8 && shm->type[ms]<13) && (i==8 || i==9 || i==11 || i==12 || i==14 || i==15))
			continue;
	//	else if((shm->type[ms]>12 && shm->type[ms]<17) && (i==))
	//		continue;
		else if((shm->type[ms]>16 && shm->type[ms]<21) && (i==5 || i==6 || i==7 || i==8 || i==9 || i==11 || i==12 || i==14 || i==15))
			continue;
		
		shm->sensorflag[ms][i]=atoi(n5);
		if(shm->sensorflag[ms][i]==1)
		{
#ifdef PRINTF
	printf("i=%d: n1=%d n2=%d n3=%d n4=%d n5=%d\n", i, atoi(n1), atoi(n2), atoi(n3), atoi(n4), atoi(n5));
#endif
			
			if(i<2)
			{
				shm->minT[ms][i] = atoi(n1);
				shm->dcritT[ms][i] = atoi(n2);
				shm->ucritT[ms][i] = atoi(n3);
				shm->maxT[ms][i] = atoi(n4);
			}
			if(i>1 && i<4)
			{
				shm->minH[ms][i-2] = atoi(n1);
				shm->dcritH[ms][i-2] = atoi(n2);
				shm->ucritH[ms][i-2] = atoi(n3);
				shm->maxH[ms][i-2] = atoi(n4);
			}
			else if(i>3 && i<10)
			{
				shm->Cmincur[ms][i-4] = atoi(n1);
				shm->Cmincrit[ms][i-4] = atoi(n2);
				shm->Cmaxcrit[ms][i-4] = atoi(n3);
				shm->Cmaxcur[ms][i-4] = atoi(n4);
			}
			else if(i>9 && i<13)
			{
				shm->Lmincur[ms][i-10] = atoi(n1);
				shm->LminCcrit[ms][i-10] = atoi(n2);
				shm->LmaxCcrit[ms][i-10] = atoi(n3);
				shm->Lmaxcur[ms][i-10] = atoi(n4);
				
			}
			else if(i>12)
			{
				shm->Lminvol[ms][i-13] = atoi(n1);
				shm->LminVcrit[ms][i-13] = atoi(n2);
				shm->LmaxVcrit[ms][i-13] = atoi(n3);
				shm->Lmaxvol[ms][i-13] = atoi(n4);
			}
			
			//写日志
			for(j=0;j<10;j++)
			{
				if(onlineper[j].ip == ri->remote_ip && ri->remote_ip)
				{
					break;
				}
			}
			if(j>=0 && j<10)
			{
				char dbinfo[255];
				memset(dbinfo,0,sizeof(char)*255);
				char dt[255];
				memset(dt,0,sizeof(char)*255);
				if(i<2)
					sprintf(dt,"%s->temperature%d",shm->devicename[ms],i+1);
				else if(i>1 && i<4)
					sprintf(dt,"%s->humidity%d",shm->devicename[ms],i-1);
				else if(i>3 && i<10)
					sprintf(dt,"%s->Loop current%d",shm->devicename[ms],i-3);
				else if(i>9 && i<13)
					sprintf(dt,"%s->Phase current%d",shm->devicename[ms],i-9);
				else if(i>12 && i<16)
					sprintf(dt,"%s->Phase voltage%d",shm->devicename[ms],i-12);
				sprintf(dbinfo,"/clever/bin/insertdb 6 %s '%s' 1",onlineper[j].name,dt);
				system(dbinfo);
			}
		}

	}
	
	
	if(ms==0)
	{
		shm->writenpminfo=1;
	}
	else
	{
		shm->slavesetflag[ms][4]=1;
	}
	
	reply_status(conn, 200, "OK");
	return;
}



static void getswitch(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i,j;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		mg_printf(conn, "failed");
		return;
	}
	
	char buf[255];
	char list[1024*2];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));	

	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	if(shm->gourpflag[ms]==0)
	{
		for(i=0;i<42;i++)
		{
			if(onlineper[gpflag].gpindex)
			{
				if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
				{
					strcat(list,"0?0?0?0?");
					continue;
				}
			}
		
			memset(buf,0,sizeof(buf));
			if(shm->type[ms]%4==2 || shm->type[ms]%4==1)
			{
				sprintf(buf,"%d?%s?%s?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],"--","--");
				strcat(list,buf);
			}
			else
			{
				sprintf(buf,"%d?%s?%d?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->swicth[ms][i],shm->swicthclock[ms][i]);
				strcat(list,buf);
			}
			
		}
		
		for(i=0;i<8;i++)
		{
			strcat(list,"0?0?0?0?");
		}
		

	}
	else if(shm->gourpflag[ms]==1)
	{
		int num=1;
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
		
		for(i=0;i<42;i++)
		{
			if(flag_1[ms][i]==1)
			{
				strcat(list,"0?0?0?0?");
			}
			else
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?0?");
						continue;
					}
				}
				if(shm->outputexist[ms][i]>0)
				{	
					memset(buf,0,sizeof(buf));
					if(shm->type[ms]%4==2 || shm->type[ms]%4==1)
					{
						sprintf(buf,"%d?%s?%s?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],"--","--");
						strcat(list,buf);
					}
					else
					{
						sprintf(buf,"%d?%s?%d?%d?",shm->outputexist[ms][i],shm->outputname[ms][i],shm->swicth[ms][i],shm->swicthclock[ms][i]);
						strcat(list,buf);
					}
					
					num++;
				}
				else
				{
					strcat(list,"0?0?0?0?");
				}
			}
			
		}
		
		for(i=0;i<8;i++)
		{
			if(flag_2[i]==1)
			{
				memset(buf,0,sizeof(buf));
				if(shm->type[ms]%4==2 || shm->type[ms]%4==1)
				{
					sprintf(buf,"%d?%s?%s?%d?",i+num,shm->groupname[ms][i],"--","--");
					strcat(list,buf);
				}
				else
				{
					sprintf(buf,"%d?%s?%d?%d?",i+num,shm->groupname[ms][i],shm->groupswicth[ms][i],shm->swicthclock[ms][42+i]);
					strcat(list,buf);
				}
			}
			else
			{
				strcat(list,"0?0?0?0?");
			}
		}
	}
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%d?",shm->swicthclock[ms][50]);
	strcat(list,buf);
	mg_printf(conn, list);
	return;
}
void RecordSwitch(int i, int num, int flag)
{
	int action= flag/10;	//开关动作	1.开 2.关 3.重启
	int index = flag%10;	//哪种方式操作开关  1.网页 2.snmp 3.telnet&ssh 4.超限断电 5.modbus 6.clever manager 7.定时开关 8.级联 9.ping
	
	char dbinfo[255];
	
	char dt[255];
	memset(dt,0,sizeof(dt));
	if(num==50)
		sprintf(dt,"%s->%s&%s&%d",shm->devicename[i],"all",action==1?"ON":(action==2?"OFF":"REBOOT"),index==1?1:(index==2?2:(index==3?3:(index==4?4:(index==5?5:(index==6?6:(index==7?7:(index==8?8:9))))))));
	else if(num<42)
		sprintf(dt,"%s->%s&%s&%d",shm->devicename[i],shm->outputname[i][num],action==1?"ON":(action==2?"OFF":"REBOOT"),index==1?1:(index==2?2:(index==3?3:(index==4?4:(index==5?5:(index==6?6:(index==7?7:(index==8?8:9))))))));
	else
		sprintf(dt,"%s->%s&%s&%d",shm->devicename[i],shm->groupname[i][num-42],action==1?"ON":(action==2?"OFF":"REBOOT"),index==1?1:(index==2?2:(index==3?3:(index==4?4:(index==5?5:(index==6?6:(index==7?7:(index==8?8:9))))))));

	memset(dbinfo,0,sizeof(dbinfo));
	sprintf(dbinfo,"/clever/bin/insertdb 11 %d '%s' 1",index,dt);
	system(dbinfo);
}
static void setswitch(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
	}

	int ms = atoi(order1);
	int index = atoi(order2);
	int action = atoi(order3);
	free(order1);
	free(order2);
	free(order3);

	if(shm->swicthclock[ms][index]==1)
	{
		shm->swicthflag[ms][index]=action*10+1;

		if(ms != 0)
			RecordSwitch(ms, index, shm->swicthflag[ms][index]);
		
		reply_status(conn, 200, "OK");
	}
	else
	{
		mg_printf(conn, "ERR");
	}
	
	
	return;
}

static void switchlock(struct mg_connection *conn, const struct mg_request_info *ri)
{

	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
	}

	int ms = atoi(order1);
	int index = atoi(order2);
	char pasd[32]={0};
	memcpy(pasd, order3, sizeof(pasd)-1);
	free(order1);
	free(order2);
	free(order3);

	int rec =-1;
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip)
		{
			if(strcmp(uinfo[i].username,onlineper[i].name)==0 && strcmp(uinfo[i].psd,pasd)==0 && strlen(pasd))
			{
				rec=0;
				break;
			}
			
		}
	}
	
	if(rec==-1)
	{
		mg_printf(conn, "err");
	}
	else
	{
		//shm->swicthclock为0时表示已锁定，1表示已开锁
		if(shm->swicthclock[ms][index]==0)
		{
			shm->swicthclock[ms][index]=1;
			if(index==50)
			{
				for(i=0;i<51;i++)
				{
					shm->swicthclock[ms][i]=1;
				}
			}
		}
		else
		{
			shm->swicthclock[ms][index]=0;
			if(index==50)
			{
				for(i=0;i<51;i++)
				{
					shm->swicthclock[ms][i]=0;
				}
			}
		}
		
		mg_printf(conn, "ok");
	}
	
	return;
}


static void getenergy(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order,0,sizeof(char)*len);
		req(request,order,0);
        free(request);
	}
	int ms = atoi(order);
	free(order);
	
	int i, j;

	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		mg_printf(conn, "failed");
		return;
	}
	
	char buf[255];
	char list[1024];
	memset(buf,0,sizeof(buf));
	memset(list,0,sizeof(list));
	
	sprintf(buf,"%d?",shm->type[ms]);
	strcat(list,buf);
	
	if(shm->gourpflag[ms]==0)
	{
		if((shm->type[ms]%4==1) || (shm->type[ms]%4==3))
		{
			for(i=0;i<50;i++)
			{
				strcat(list,"0?0?0?");
			}
			
			if(shm->type[ms]==1 || shm->type[ms]==3)
			{
				for(i=0;i<2;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
				for(i=0;i<4;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==5 || shm->type[ms]==7)
			{
				for(i=0;i<3;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
				for(i=0;i<3;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==9 || shm->type[ms]==11)
			{
				for(i=0;i<4;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
				for(i=0;i<3;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==13 || shm->type[ms]==15)
			{
				for(i=0;i<6;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
			}
			else if(shm->type[ms]==17 || shm->type[ms]==19)
			{
				for(i=0;i<1;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
				for(i=0;i<5;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
		}
		else
		{
			for(i=0;i<42;i++)
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
					{
						strcat(list,"0?0?0?");
						continue;
					}
				}
				
				memset(buf,0,sizeof(buf));
				sprintf(buf,"%d?%s?%.1f?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->kwh[ms][i]/10);
				strcat(list,buf);
				
			}
			
			for(i=0;i<8;i++)
			{
				strcat(list,"0?0?0?");
			}
			
			for(i=0;i<6;i++)
			{
				strcat(list,"0?0?0?");
			}
		}
	}
	else if(shm->gourpflag[ms]==1)
	{
		int num=1;
		int flag_1[10][42]={0};
		int flag_2[8]={0};
		
		for(i=0;i<8;i++)
		{
			for(j=0;j<42;j++)
			{
				if(shm->group[ms][i][j]==1)
				{
					flag_1[ms][j]=1;
					flag_2[i]=1;
				}
			}
		}
		
		if((shm->type[ms]%4==1) || (shm->type[ms]%4==3))
		{
			for(i=0;i<50;i++)
			{
				strcat(list,"0?0?0?");
			}
			
			if(shm->type[ms]==1 || shm->type[ms]==3)
			{
				for(i=0;i<2;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,((float)shm->Ckwh[ms][i])/10);
					strcat(list,buf);
				}
				for(i=0;i<4;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==5 || shm->type[ms]==7)
			{
				for(i=0;i<3;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,((float)shm->Ckwh[ms][i])/10);
					strcat(list,buf);
				}
				for(i=0;i<3;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==9 || shm->type[ms]==11)
			{
				for(i=0;i<4;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,((float)shm->Ckwh[ms][i])/10);
					strcat(list,buf);
				}
				for(i=0;i<3;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
			else if(shm->type[ms]==13 || shm->type[ms]==15)
			{
				for(i=0;i<6;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,((float)shm->Ckwh[ms][i])/10);
					strcat(list,buf);
				}
			}
			else if(shm->type[ms]==17 || shm->type[ms]==19)
			{
				for(i=0;i<1;i++)
				{
					memset(buf,0,sizeof(buf));
					sprintf(buf,"%d?C%d?%.2f?",i+1,i+1,(float)shm->Ckwh[ms][i]/10);
					strcat(list,buf);
				}
				for(i=0;i<5;i++)
				{
					strcat(list,"0?0?0?");
				}
			}
		}
		else
		{
			for(i=0;i<42;i++)
			{
				if(flag_1[ms][i]==1)
				{
					strcat(list,"0?0?0?");
				}
				else
				{
					if(onlineper[gpflag].gpindex)
					{
						if(gp[onlineper[gpflag].gpindex].outputper[ms][i]==0)
						{
							strcat(list,"0?0?0?");
							continue;
						}
					}
					if(shm->outputexist[ms][i]>0)
					{
						memset(buf,0,sizeof(char)*255);
						sprintf(buf,"%d?%s?%.1f?",shm->outputexist[ms][i],shm->outputname[ms][i],(float)shm->kwh[ms][i]/10);
						strcat(list,buf);	
						
						num++;
					}
					else
					{
						strcat(list,"0?0?0?");
					}
				}
			}

			for(i=0;i<8;i++)
			{
				if(flag_2[i]==1)
				{
					memset(buf,0,sizeof(char)*255);
					sprintf(buf,"%d?%s?%.2f?",num,shm->groupname[ms][i],((float)shm->gpkwh[ms][i])/10);
					strcat(list,buf);	
					
					num++;
				}
				else
				{
					strcat(list,"0?0?0?");
				}
			}
			
			for(i=0;i<6;i++)
			{
				strcat(list,"0?0?0?");
			}
		}
	}

	mg_printf(conn, list);
	return;
}

static void setenergy(struct mg_connection *conn, const struct mg_request_info *ri)
{	
	int a,n;
	n=0;
	for(a=0;a<10;a++)
	{
		if(strcmp(onlineper[a].name,"admin")==0)
		{	
			
			n=1;
			break;

		}
		
	}
	if(n == 0 ){
		
		mg_printf(conn,"not admin user");
		return;
	}
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
        free(request);
	}
	int ms = atoi(order1);
	int index = atoi(order2);
	free(order1);
	free(order2);
	int i;

	shm->energcleary[ms][index]=1;

	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		char dt[255];
		memset(dt,0,sizeof(char)*255);
		if(index<42)
			sprintf(dt,"%s->%s",shm->devicename[ms],shm->outputname[ms][index]);
		else if(index>41 && index<50)
			sprintf(dt,"%s->%s",shm->devicename[ms],shm->groupname[ms][index-42]);
		else if(index>49 && index<56)
			sprintf(dt,"%s->Total energy%d",shm->devicename[ms],index-50);
		else if(index==56)
			sprintf(dt,"%s->All energy",shm->devicename[ms]);
		sprintf(dbinfo,"/clever/bin/insertdb 7 %s '%s' 1",onlineper[i].name,dt);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 2 'Device Settings:' '%s(Device name->Output name)electric energy metering is reset.'",dt);
		system(sys_info);
	}
	reply_status(conn, 200, "OK");
	return;		
}

static void geteth0info(struct mg_connection *conn, const struct mg_request_info *ri)
{
	int i;
	char line[255];
	char order[255];
	char buf[1024];
	
	memset(buf,0,sizeof(buf));
	FILE *file = NULL;
	file = popen("sed -n '1p' /clever/conf/eth0-setting | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	
	if(atoi(line)==0)
	{
		for(i=0;i<5;i++)
		{
			memset(order,0,sizeof(order));
			sprintf(order,"sed -n '%dp' /clever/conf/eth0-setting | cut -d= -f2",i+2);
			memset(line,0,sizeof(line));
			file = popen(order, "r");
			if(file!= NULL)
			{
				if (fgets(line, 255, file) == NULL)
				{
					mg_printf(conn, "err1");
					pclose(file);
					return;
				}
			}
			pclose(file);
			strcat(buf,line);
			strcat(buf,"?");
		}
	}
	else 
	{
		memset(order,0,sizeof(order));
		sprintf(order,"ifconfig |grep 'inet addr:' | grep -v '127.0.0.1' |cut -d: -f2 |busybox awk '{print $1}'");
		memset(line,0,sizeof(line));
		file = popen(order, "r");
		if (file != NULL)
		{
			if (fgets(line, 32, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");

		strcat(buf,"\0?\0?\0?\0?");
	}
	mg_printf(conn,buf);
	
}

static void setIPv4(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
        free(request);
	}
	char order[255];
	int i;
	if(strcmp(order1,"1")==0)
	{
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '1cAUTOIP=1' /clever/conf/eth0-setting");
		system(order);
	}
	else
	{
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '1cAUTOIP=0' /clever/conf/eth0-setting");
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '2cIP=%s' /clever/conf/eth0-setting",order2);
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '3cMask=%s' /clever/conf/eth0-setting",order3);
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '4cGateway=%s' /clever/conf/eth0-setting",order4);
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '5cDNS=%s' /clever/conf/eth0-setting",order5);
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '6cDNS1=%s' /clever/conf/eth0-setting",order6);
		system(order);
		
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '2cnameserver %s' /clever/conf/resolv.conf",order5);
		system(order);
		memset(order,0,sizeof(order));
		sprintf(order,"sed -i '3cnameserver %s' /clever/conf/resolv.conf",order6);
		system(order);
		
		system("sync");
	}
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 12 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s Network configuration was successfully modified.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	reply_status(conn, 200, "OK");
}

/**wifi.html**/
static void getwlan0info(struct mg_connection *conn, const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[1024];
	int i;
	memset(buf,0,sizeof(char)*1024);
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -n '%dp' /clever/conf/eth0-setting | cut -d= -f2",3);
	memset(line,0,sizeof(char)*255);
	file = popen(order, "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	for(i=0;i<2;i++)
	{
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -n '%dp' /clever/conf/resolv.conf | cut -d' ' -f2",i+2);
		memset(line,0,sizeof(char)*255);
		file = popen(order, "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");
	}
	for(i=0;i<2;i++)
	{
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -n '%dp' /clever/conf/eth0-setting | cut -d= -f2",i+6);
		memset(line,0,sizeof(char)*255);
		file = popen(order, "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");
	}
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '1p' /clever/conf/wlan0-setting | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '2p' /clever/conf/wlan0-setting | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '40p' /etc/wpa_supplicant.conf | cut -d\\\" -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '45p' /etc/wpa_supplicant.conf | cut -d\\\" -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	mg_printf(conn,buf);
}

static void searchwlan0(struct mg_connection *conn, const struct mg_request_info *ri)
{
	system("ifconfig wlan0 up");
	char buf[1024];
	memset(buf,0,sizeof(char)*1024);
	FILE *file = NULL;
	file = popen("iwlist wlan0 scanning | grep 'ESSID'", "r");
	if(file!= NULL)
	{
		int i=0;
		while(i<200)
		{
			int ch;
			ch = fgetc(file);
			if(ch==EOF)
			{
				break;
			}
			if(ch>0 && ch <128)
			{
				buf[i]=ch;
				i++;
			}
		}
	}
	pclose(file);
	system("ifconfig wlan0 down");
	mg_printf(conn,buf);
}

static void setwlan0id(struct mg_connection *conn,  const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
	}
	char order[255];
	if(strcmp(order1,"1")==0)
	{
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '7cETH0=1' /clever/conf/eth0-setting");
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '40cssid=\"%s\"' /etc/wpa_supplicant.conf",order2);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '45cpsk=\"%s\"' /etc/wpa_supplicant.conf",order3);
		system(order);
	}
	else
	{
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '7cETH0=0' /clever/conf/eth0-setting");
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '40cssid=\"%s\"' /etc/wpa_supplicant.conf",order2);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '45cpsk=\"%s\"' /etc/wpa_supplicant.conf",order3);
		system(order);
	}
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 14 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s WIFI configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	system("sync");
	mg_printf(conn, "OK");
}
static void setwlan0info(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
        free(request);
	}
	char order[255];
    if(strcmp(order1,"1")==0)
    {
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '6cAUTOIP=1' /clever/conf/eth0-setting");
		system(order);
    }
    else
    {
    	memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '6cAUTOIP=0' /clever/conf/eth0-setting");
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '1cWIP=%s' /clever/conf/wlan0-setting",order2);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '2cWMK=%s' /clever/conf/wlan0-setting",order3);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '3cGateway=%s' /clever/conf/eth0-setting",order4);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '2cnameserver %s' /clever/conf/resolv.conf",order5);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '3cnameserver %s' /clever/conf/resolv.conf",order6);
		system(order);
		system("sync");
    }
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 15 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s Network configuration was successfully modified.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	reply_status(conn, 200, "OK");
}
static void getsnmpinfo(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[1024];
	memset(buf,0,sizeof(char)*1024);
	
	memset(line,0,sizeof(line));
	sprintf(line,"%d?",shm->Language);
	strcat(buf,line);
	
	
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '18p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '19p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '61p' /clever/conf/snmpd.conf | cut -d' ' -f4", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '62p' /clever/conf/snmpd.conf | cut -d' ' -f4", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '16p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '17p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '125p' /clever/conf/snmpd.conf | cut -d' ' -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '126p' /clever/conf/snmpd.conf | cut -d' ' -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '92p' /clever/conf/snmpd.conf | cut -d' ' -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '92p' /clever/conf/snmpd.conf | cut -d\\\" -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '92p' /clever/conf/snmpd.conf | cut -d' ' -f6", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	// memset(line,0,sizeof(char)*255);
	// file = popen("sed -n '1p' /clever/conf/trap | cut -d= -f2", "r");
	// if(file!= NULL)
	// {
	// 	if (fgets(line, 255, file) == NULL)
	// 	{
	// 		mg_printf(conn, "err1");
	// 		pclose(file);
	// 		return;
	// 	}
	// }
	// pclose(file);
	// strcat(buf,line);
	// strcat(buf,"?");

	mg_printf(conn,buf);
}
static void setsnmp(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	char *order7 = (char *)malloc(sizeof(char)*len);
	// char *order8 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		// memset(order8,0,sizeof(char)*len);
		// req(request,order8,7);
        free(request);
	}
	char order[255];
	if(strncmp(order1,"0",1)==0)
	{
		system("sed -i '18csnmpd=1' /clever/conf/device");
	}
	else
	{
		system("sed -i '18csnmpd=0' /clever/conf/device");
	}
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '61ccom2sec local default %s' /clever/conf/snmpd.conf",order2);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '62ccom2sec mynetwork default %s' /clever/conf/snmpd.conf",order3);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '16ctrap1=%s' /clever/conf/device",order4);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '17ctrap2=%s' /clever/conf/device",order5);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '125csyslocation %s' /clever/conf/snmpd.conf",order6);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '126csyscontact %s' /clever/conf/snmpd.conf",order7);
	system(order);
	// memset(order,0,sizeof(char)*255);
	// sprintf(order,"sed -i '1ctrap=%s' /clever/conf/trap",order8);
	// system(order);
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 25 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s SNMP configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	free(order7);
	// free(order8);
	system("sync");
	reply_status(conn, 200, "OK");
}
static void setsnmpv3(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
        free(request);
	}
	char order[255];
	if(strncmp(order1,"0",1)==0)
	{
		system("sed -i '19csnmpv3=1' /clever/conf/device");
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '92ccreateUser %s MD5 \"%s\" DES %s' /clever/conf/snmpd.conf",
				order2,order3,order4);
		system(order);
	}
	else
	{
		system("sed -i '19csnmpv3=0' /clever/conf/device");
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '92c#createUser %s MD5 \"%s\" DES %s' /clever/conf/snmpd.conf",
				order2,order3,order4);
		system(order);
	}
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '93crwuser %s' /clever/conf/snmpd.conf",
			order2);
	system(order);
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 26 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s SNMP configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	system("sync");
	reply_status(conn, 200, "OK");
}
static void getsmtpinfo(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[1024];
	int i;
	memset(buf,0,sizeof(char)*1024);
	for(i=0;i<5;i++)
	{
		char order[255];
		memset(order,0,sizeof(char)*255);
		memset(line,0,sizeof(char)*255);
		sprintf(order,"sed -n '%dp' /clever/conf/device | cut -d= -f2",i+9);
		file = popen(order, "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");
	}
	mg_printf(conn,buf);
}
static void setsmtp(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
        free(request);
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '9csmtpuser=%s' /clever/conf/device",order1);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '10csmtppsd=%s' /clever/conf/device",order2);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '11csmtpserver=%s' /clever/conf/device",order3);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '12csmtpport=%s' /clever/conf/device",order4);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '13csmtpauth=%s' /clever/conf/device",order5);
	system(order);
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 21 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s SMTP configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	system("sync");
	reply_status(conn, 200, "OK");
}
static void smtptest(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
        free(request);
	}
	char order[1024];
	memset(order,0,sizeof(char)*1024);
	sprintf(order,"/clever/bin/smtp %s %s %s %s %s %s %s %s"
			,order1
			,order2
			,order3
			,order4
			,order6
			,order5
			,"SmtpTest"
			,"SmtpTest"
			);
	system(order);
	reply_status(conn, 200, "OK");
}
static void getntpinfo(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[1024];
	char order[255];
	int i;
	memset(buf,0,sizeof(char)*1024);
	for(i=0;i<3;i++)
	{
		memset(order,0,sizeof(char)*255);
		memset(line,0,sizeof(char)*255);
		sprintf(order,"sed -n '%dp' /clever/conf/device | cut -d= -f2",i+3);
		file = popen(order, "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");
	}

	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '20p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
	time_t now;
	time(&now);
	struct tm *ct;
	ct = gmtime(&now);
	char tmdata[32];
	memset(tmdata,0,sizeof(char)*32);
	sprintf(tmdata,"%04d-%02d-%02d",ct->tm_year+1900,ct->tm_mon+1,ct->tm_mday);
	strcat(buf,tmdata);
	strcat(buf,"?");
	memset(tmdata,0,sizeof(char)*32);
	sprintf(tmdata,"%02d:%02d",ct->tm_hour,ct->tm_min);
	strcat(buf,tmdata);
	strcat(buf,"?");
	mg_printf(conn,buf);
}
static void setntp(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
        free(request);
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '20cntpenable=%s' /clever/conf/device",strcmp(order1,"0")==0?"1":"0");
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '4cntps=%s' /clever/conf/device",order2);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '3cntpp=%s' /clever/conf/device",order3);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '5czone=%s' /clever/conf/device",order4);
	system(order);
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 18 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s NTP configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	system("sync");
	reply_status(conn, 200, "OK");
}

static void ntpsync(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"/clever/bin/sntp %s %s %s",order1,order2,order3);
	system(order);
	free(order1);
	free(order2);
	free(order3);
	reply_status(conn, 200, "OK");
}
static void settm(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
        free(request);
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"date -s '%s %s'",order1,order2);
	system(order);
	mg_printf(conn, "%s",order);
	system("hwclock -w");	//system("hwclock --systohc --utc");写系统时间到硬件时间   system("hwclock -r");读硬件时间
	free(order1);
	free(order2);
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 19 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Device Settings:' '%s Clock was reset.'",onlineper[i].name);
		system(sys_info);
	}
	system("sync");
	reply_status(conn, 200, "OK");
}


static void operatelog(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
        free(request);
	}
	int pg = atoi(order1);
	free(order1);
	
	int rc;
	sqlite3 * db=NULL;
	int counter;
	char **resuilt;
	int row;
	int colnum;
	sqlite3_stmt *sta;
	rc = sqlite3_open("/clever/log/operatelog.db",&db);
	sqlite3_exec(db,"create table dblog (tm varchar(20), type int, name varchar(32),detail varchar(128));",0,0,NULL);
	if(rc)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	rc=sqlite3_prepare(db,"select count(*) from dblog;",-1,&sta,0);
	if(rc != SQLITE_OK)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	counter=0;
	if(sqlite3_step(sta)==SQLITE_ROW)
	{
		counter = sqlite3_column_int(sta,0);
	}
	sqlite3_finalize(sta);
	if(pg<1)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	if((pg-1)*24>counter)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	int pagenum;
	if(counter%24)
	{
		pagenum= counter/24+1;
	}
	else
	{
		pagenum= counter/24;
	}
	char list[4096];
	char search[255];
	memset(search,0,sizeof(char)*255);
	memset(list,0,sizeof(char)*4096);
	if(pg*24>=counter && pg*24-counter<=24)
	{
		sprintf(search,"select * from dblog limit 0,%d;",(counter-(pg-1)*24));
	}
	else
	{
		sprintf(search,"select * from dblog limit %d,24;",(counter-pg*24));
	}
	
	rc=sqlite3_get_table(db,search,&resuilt,&row,&colnum,NULL);
	if(rc == SQLITE_OK)
	{
		int i,j;
		int index = 0;
		char pagechar[32];
		memset(pagechar,0,sizeof(char)*32);
		sprintf(pagechar,"%d",pagenum);
		strcat(list,pagechar);
		strcat(list,"?");
		for(i=row;i>0;i--)
		{
			index=i*colnum;
			for(j=0;j<colnum;j++)
			{
				strcat(list,resuilt[index]);
				strcat(list,"?");
				++index;
			}
		}
	}
	else
	{
		sqlite3_free_table(resuilt);
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	sqlite3_free_table(resuilt);
	sqlite3_close(db);

	mg_printf(conn, list);
}

static void alarmlog(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
        free(request);
	}
	int pg = atoi(order1);
	free(order1);
	
	int rc;
	sqlite3 * db=NULL;
	int counter;
	char **resuilt;
	int row;
	int colnum;
	sqlite3_stmt *sta;
	rc = sqlite3_open("/clever/log/alarmlog.db",&db);
	sqlite3_exec(db,"create table dblog (tm varchar(20), type int, name varchar(32),detail varchar(128));",0,0,NULL);
	if(rc)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	rc=sqlite3_prepare(db,"select count(*) from dblog;",-1,&sta,0);
	if(rc != SQLITE_OK)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	counter=0;
	if(sqlite3_step(sta)==SQLITE_ROW)
	{
		counter = sqlite3_column_int(sta,0);
	}
	sqlite3_finalize(sta);
	if(pg<1)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	if((pg-1)*24>counter)
	{
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	int pagenum;
	if(counter%24)
	{
		pagenum= counter/24+1;
	}
	else
	{
		pagenum= counter/24;
	}
	char list[4096];
	char search[255];
	memset(search,0,sizeof(char)*255);
	memset(list,0,sizeof(char)*4096);
	if(pg*24>=counter && pg*24-counter<=24)
	{
		sprintf(search,"select * from dblog limit 0,%d;",(counter-(pg-1)*24));
	}
	else
	{
		sprintf(search,"select * from dblog limit %d,24;",(counter-pg*24));
	}
	
	rc=sqlite3_get_table(db,search,&resuilt,&row,&colnum,NULL);
	if(rc == SQLITE_OK)
	{
		int i,j;
		int index = 0;
		char pagechar[32];
		memset(pagechar,0,sizeof(char)*32);
		sprintf(pagechar,"%d",pagenum);
		strcat(list,pagechar);
		strcat(list,"?");
		for(i=row;i>0;i--)
		{
			index=i*colnum;
			for(j=0;j<colnum;j++)
			{
				strcat(list,resuilt[index]);
				strcat(list,"?");
				++index;
			}
		}
	}
	else
	{
		sqlite3_free_table(resuilt);
		sqlite3_close(db);
		mg_printf(conn, "err1");
		return;
	}
	sqlite3_free_table(resuilt);
	sqlite3_close(db);
	mg_printf(conn, list);
}


static void getsyslog(struct mg_connection *conn, const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	memset(line,0,sizeof(char)*255);
	
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	
	file = popen("sed -n '30p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	
	file = popen("sed -n '1p' /clever/conf/syslog_protity | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	
	
	
	file = popen("sed -n '2p' /clever/conf/syslog_protity | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	
	file = popen("sed -n '3p' /clever/conf/syslog_protity | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	
	file = popen("sed -n '4p' /clever/conf/syslog_protity | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");

	mg_printf(conn,buf);
}
static void setsyslog(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	
	char *p_req =request;
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		
        free(request);
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '30csyslogserver=%s' /clever/conf/device",order1);
	system(order);
	free(order1);
	
	int index_i=0;
	index_i=atoi(order2);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1cSensor_Detection=%d' /clever/conf/syslog_protity",index_i);//SENSOR

	system(order);
	
	index_i=atoi(order3);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '2cDevice_Settings=%d' /clever/conf/syslog_protity",index_i);//SENSOR
	system(order);
	
	index_i=atoi(order4);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '3cUser_Adminstor=%d' /clever/conf/syslog_protity",index_i);//SENSOR_ALARM
	
	system(order);
	
	index_i=atoi(order5);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '4cNetwork_Settings=%d' /clever/conf/syslog_protity",index_i);//SENSOR_ALARM
	
	system(order);
	
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 28 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s SYSLOG configuration was changed.'",onlineper[i].name);
		system(sys_info);
		
	}
	system("sync");
	reply_status(conn, 200, "OK");
}
static void clearlogs(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,2))
	{
		mg_printf(conn, "flase");
		return;
	}
	system("/clever/bin/insertdb 100 0 0 1");
	
	char sys_info[255];
	memset(sys_info,0,sizeof(char)*255);
	sprintf(sys_info,"/clever/bin/syslog_send 3 'log Settings:' 'clear logs.'");
	system(sys_info);
	reply_status(conn, 200, "OK");
}

static void clearalarmlogs(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,2))
	{
		mg_printf(conn, "flase");
		return;
	}
	system("/clever/bin/insertdb 100 0 0 2");
	
	char sys_info[255];
	memset(sys_info,0,sizeof(char)*255);
	sprintf(sys_info,"/clever/bin/syslog_send 3 'log Settings:' 'clear alarm logs.'");
	system(sys_info);
	reply_status(conn, 200, "OK");
}


static void getweb(struct mg_connection *conn, const struct mg_request_info *ri)
{
	FILE *file = NULL;
    char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '2p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	file = popen("sed -n '21p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	file = popen("sed -n '15p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	mg_printf(conn,buf);
}
static void setweb(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
        free(request);
	}
	int port1=atoi(order1);
	int port2=atoi(order2);
	int enssl=atoi(order3);
	free(order1);
	free(order2);
	free(order3);
	if(enssl<0 || enssl>1)
        {
		mg_printf(conn, "err1");
		return;
	}
	if(port1<0 || port1>65535 || port2<0 || port2>65535)
        {
		mg_printf(conn, "err1");
		return;
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '2cbport=%d' /clever/conf/device",port1);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '21csslport=%d' /clever/conf/device",port2);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '15censsl=%d' /clever/conf/device",enssl);
	system(order);
	system("sync");
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 17 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s HTTP configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	
	reply_status(conn, 200, "OK");
}
static void gettelnet(struct mg_connection *conn, const struct mg_request_info *ri)
{
	FILE *file = NULL;
        char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	memset(line,0,sizeof(char)*255);
	file = popen("sed -n '24p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	file = popen("sed -n '7p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	file = popen("sed -n '8p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	file = popen("sed -n '6p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	mg_printf(conn,buf);
}
static void settelnet(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
        free(request);
	}
	int telen=atoi(order1);
	int port=atoi(order4);
	char nm[255];
	char psd[255];
	memset(nm,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	strncpy(nm,order2,sizeof(char)*30);
	strncpy(psd,order3,sizeof(char)*30);
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	if(telen<0 || telen>1)
        {
		mg_printf(conn, "err1");
		return;
	}
	if(port<0 || port>65535)
        {
		mg_printf(conn, "err1");
		return;
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '24ctelen=%d' /clever/conf/device",telen==0?1:0);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '7cteluser=%s' /clever/conf/device",nm);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '8ctelpsd=%s' /clever/conf/device",psd);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '6ctelnetp=%d' /clever/conf/device",port);
	system(order);
	system("sync");
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 21 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s telnet configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	
	reply_status(conn, 200, "OK");
}

int change_permission(char *nm)
{	
	FILE *sfile;
	if((sfile=fopen("/etc/passwd","r+"))==NULL)
	{
		return -1;
	}
	char buf[256]={0};
	long prepos;
	do{
		int fnum=0;
		int fixflag=0;
		prepos=ftell(sfile);
		memset(buf,0,sizeof(buf));
		int pstart=0;
		int pend=0;
		if(fgets(buf,sizeof(buf),sfile)){
			int i=0;
			for(i=0;i<sizeof(buf);i++)
			{
				if(buf[i]==':')
				{
					fnum++;
					if(fnum==1)
					{
						buf[i]='\0';
						if(!strcmp(nm,buf))
						{
							fixflag=1;
						}else
						{
							break;
						}
					}else if(fnum==2 && fixflag==1)
					{
						pstart=prepos+i+1;
					}else if(fnum==3 && fixflag ==1)
					{
						pend=prepos+i;
						fseek(sfile,pstart,SEEK_SET);
						int j=0;
						char a='0';
						for(j=0;j<pend-pstart;j++)
						{
							if(fwrite(&a,sizeof(a),1,sfile) !=1) 
							{
								fclose(sfile);
								return -1;
							}
						}
						break;
					}
				}
			}
		}
		if(fnum==3 && fixflag==1)
			break;
	}while(!feof(sfile));
	fclose(sfile);
	return 0;
}
/*2014-08-15 when the date is 1970-01-01 generator /etc/shadow is  :0:0:99999:7:::
 *ssh can't connect .I change it to  :1:0:99999:7:::
 * the funtion parameter is username;
 *write by:zhangwujiang
 */
int change_shadow(char *nm)
{	
	FILE *sfile;
	if((sfile=fopen("/etc/shadow","r+"))==NULL)
	{
		return -1;
	}
	char buf[256]={0};
	long prepos;
	do{
		int fnum=0;
		int fixflag=0;
		prepos=ftell(sfile);
		memset(buf,0,sizeof(buf));
		if(fgets(buf,sizeof(buf),sfile)){
			int i=0;
			for(i=0;i<sizeof(buf);i++)
			{
				if(buf[i]==':')
				{
					fnum++;
					if(fnum==1)
					{
						buf[i]='\0';
						if(!strcmp(nm,buf))
						{
							fixflag=1;
						}
						else
						{
							break;
						}
					}else if(fnum==2 && fixflag==1)
					{
						fixflag=0;
						char aa;
						fseek(sfile,prepos+i+1,SEEK_SET);
						if(fread(&aa,sizeof(aa),1,sfile)==1)
						{
							if(aa=='0')
							{
								aa='1';
								fseek(sfile,prepos+i+1,SEEK_SET);
								if(fwrite(&aa,sizeof(aa),1,sfile) !=1)
								{
									fclose(sfile);
									return -1;
								}
							}
						}
						break;
					}
				}
			}
		}
		if(fnum==2)
			break;
	}while(!feof(sfile));
	fclose(sfile);
	return 0;
}

/*2014-08-13 add account �� password for linux
 *write by :zhangwujiang
 */
int ssh_add_user_passwd(char *nm,char *psd)
{
	if(nm==NULL || psd==NULL)
	{
		return -1;
	}
	char adduser[255];
	memset(adduser,0,sizeof(char)*255);
	//sshd account name must different from ftp account

	FILE *file;
	int len;
	char line[255];
	memset(line,0,sizeof(char)*255);
	//get ftp account name
	file = popen("sed -n '37p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return 0;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;

	//when the name is "", "root" "nobody(ftp used)", "sshd" line return
	if(nm[0]=='\0' || !strcmp(nm,"root")  || !strcmp(nm,"sshd")  || !strcmp(nm,"nobody")  || !strcmp(nm,line))
	{
		return -1;
	}
	else
	{

		//before add user,we delete the preuser
		//FILE *file = NULL;
		//char line[255];
		//int len;
		memset(line,0,sizeof(char)*255);
		//get ssh prename
		file = popen("sed -n '33p' /clever/conf/device | cut -d= -f2", "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				pclose(file);
				return -1;
			}
		}
		pclose(file);
		len = strlen(line);
		if(len)
		{
			line[len-1]='\0';
		}
		//delete the preuser
		if(line[0] !='\0')
		{
			memset(adduser,0,sizeof(char)*255);
		//	sprintf(adduser,"deluser %s",line);
			sprintf(adduser,"busybox deluser %s",line);	//QLL
			system(adduser);

			memset(adduser,0,sizeof(char)*255);
		//	sprintf(adduser,"rm /home/%s -rf",line);
			sprintf(adduser,"busybox rm /home/%s -rf",line);//QLL
			system(adduser);
		}

		//add user name=nm, home directory=/home, shell=/clever/bin/sshcmd  no input pasword
		memset(adduser,0,sizeof(char)*255);
	//	sprintf(adduser,"adduser  -s /clever/bin/sshcmd -D %s",nm);
		sprintf(adduser,"busybox adduser  -s /clever/bin/sshcmd -D %s",nm);//QLL
		system(adduser);
		memset(adduser,0,sizeof(char)*255);
		//change passwd 
		sprintf(adduser,"(echo %s; sleep 1;echo %s) | passwd %s",psd,psd,nm);
		system(adduser);

		if(change_shadow(nm) != 0)
		{
			return -1;
		}	
	}
	return 0;
}

/*2014-08-15 when reset happan,the preuser didn't delete,this will do that
 *write by: zhangwujiang
 */
void initssh()
{
	char nm[255];
	char psd[255];
	char prenm[255];
	char prepsd[255];
	char line[255];
	char adduser[255];
	memset(nm,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	memset(prenm,0,sizeof(char)*255);
	memset(prepsd,0,sizeof(char)*255);	
	memset(adduser,0,sizeof(char)*255);
	
	system("mount -o remount,rw /");	//QLL
	system("busybox mkdir /home");		//QLL
	system("busybox chmod 777 /clever/bin/sshcmd");	//QLL
	
	//get ssh name
	FILE *file;
	int len;
	file = popen("sed -n '33p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(nm,line,sizeof(char)*255);
	//get ssh password
	file = popen("sed -n '34p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(psd,line,sizeof(char)*255);

	file = popen("sed -n '3p' /clever/conf/usercopy | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(prenm,line,sizeof(char)*255);

	file = popen("sed -n '4p' /clever/conf/usercopy | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(prepsd,line,sizeof(char)*255);
	//when the name is "", "root", "sshd" "nobody" or the name didn't change , jump adduser and exit.
	if(nm[0]=='\0' || !strcmp(nm,"root") || !strcmp(nm,"sshd") || !strcmp(nm,"nobody")  || (!strcmp(nm,prenm) && !strcmp(psd,prepsd)))
	{
		return;
	}
	//add user name=nm, home directory=/home/nm, shell=/clever/bin/sshcmd  no input pasword
	else
	{
		if(prenm[0] !='\0')
		{
			memset(adduser,0,sizeof(char)*255);
		//	sprintf(adduser,"deluser %s",prenm);
			sprintf(adduser,"busybox deluser %s",prenm);//QLL
			system(adduser);
			//delete the prenm's home directory 		     
			memset(adduser,0,sizeof(char)*255);
		//	sprintf(adduser,"rm /home/%s -rf",prenm);
			sprintf(adduser,"busybox rm /home/%s -rf",prenm);//QLL
			system(adduser);
		}
		memset(adduser,0,sizeof(char)*255);
	//	sprintf(adduser,"adduser  -s /clever/bin/sshcmd -D %s",nm);
		sprintf(adduser,"busybox adduser  -s /clever/bin/sshcmd -D %s",nm);
		system(adduser);
		memset(adduser,0,sizeof(char)*255);
		sprintf(adduser,"(echo %s; sleep 1;echo %s) | passwd %s",psd,psd,nm);
		system(adduser);
		change_shadow(nm);
		change_permission(nm);

		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '3csshuser=%s' /clever/conf/usercopy",nm);
		system(line);
		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '4csshpsd=%s' /clever/conf/usercopy",psd);
		system(line);

		//only allow nm login
		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '1cAllowUsers %s' /usr/local/etc/sshd_config",nm);
		system(line);

		system("sync");

	}

}



/*2014-08-13 let the web show SSH account,SSH password
 *write by: zhangwujiang
 */
static void getssh(struct mg_connection *conn,
		const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	memset(line,0,sizeof(char)*255);
	//is ssh enable?
	file = popen("sed -n '32p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ssh name
	file = popen("sed -n '33p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}

	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ssh password
	file = popen("sed -n '34p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}

	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ssh port number 22
	file = popen("sed -n '35p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	mg_printf(conn,buf);
}
/*2014-08-13 set SSH account,SSH password
 *write by: zhangwujiang
 */
static void setssh(struct mg_connection *conn,
		const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	// enable\ name\ password\ port
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		free(request);
	}
	int sshen=atoi(order1);
	int port=atoi(order4);
	char nm[255];
	char psd[255];
	memset(nm,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	strncpy(nm,order2,sizeof(char)*30);
	strncpy(psd,order3,sizeof(char)*30);
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	if(sshen<0 || sshen>1)
	{
		mg_printf(conn, "err1");
		return;
	}
	if(port<0 || port>65535)
	{
		mg_printf(conn, "err1");
		return;
	}

	/*2014-08-15 add account for ssh in linux
	 *write by:zhangwujiang
	 */
	if(ssh_add_user_passwd(nm,psd) != 0)
	{
		mg_printf(conn, "err1");
		return;
	}
	if(change_permission(nm) != 0)
	{
		mg_printf(conn, "err1");
		return;
	}


	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '32csshen=%d' /clever/conf/device",sshen==0?1:0);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '33csshuser=%s' /clever/conf/device",nm);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '34csshpsd=%s' /clever/conf/device",psd);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '35csshp=%d' /clever/conf/device",port);
	system(order);

	/*2014-09-12 save the name and password to another file
	 *write by: zhang wujiang
	 */
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '3csshuser=%s' /clever/conf/usercopy",nm);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '4csshpsd=%s' /clever/conf/usercopy",psd);
	system(order);

	//only allow nm login
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1cAllowUsers %s' /usr/local/etc/sshd_config",nm);
	system(order);

	system("sync");
	reply_status(conn, 200, "OK");
}


static void getradius(struct mg_connection *conn,
        const struct mg_request_info *ri)
{
	int i;
	FILE *file = NULL;
        char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	for(i=25;i<30;i++)
	{
		char order[255];
		file = NULL;
		memset(line,0,sizeof(char)*255);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -n '%dp' /clever/conf/device | cut -d= -f2",i);
		file = popen(order, "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				mg_printf(conn, "err1");
				pclose(file);
				return;
			}
		}
		pclose(file);
		int len = strlen(line);
		if(len)
		{
			line[len-1]='\0';
		}
		strcat(buf,line);
		strcat(buf,"?");
	}
	mg_printf(conn,buf);
}
static void setradius(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
        free(request);
	}
	int rad=atoi(order1);
	int port1=atoi(order4);
	int port2=atoi(order5);
	char server[255];
	char psd[255];
	memset(server,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	strcpy(server,order2);
	strcpy(psd,order3);
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	if(rad<0 || rad>2)
        {
		mg_printf(conn, "err1");
		return;
	}
	if(port1<0 || port1>65535)
        {
		mg_printf(conn, "err1");
		return;
	}
	if(port2<0 || port2>65535)
        {
		mg_printf(conn, "err1");
		return;
	}
	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '25cradius=%d' /clever/conf/device",rad);
	system(order);
	if(rad==1 || rad == 2)
	{
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '26cradserver=%s' /clever/conf/device",server);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '27cradsecret=%s' /clever/conf/device",psd);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '28cradathport=%d' /clever/conf/device",port1);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '29cradaccport=%d' /clever/conf/device",port2);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '37cauthserver 	%s:%d:%s' /etc/radiusclient/radiusclient.conf",server,port1,psd);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '42cacctserver 	%s:%d:%s' /etc/radiusclient/radiusclient.conf",server,port2,psd);
		system(order);
		memset(order,0,sizeof(char)*255);
		sprintf(order,"sed -i '11c%s	%s' /etc/radiusclient/servers",server,psd);
		system(order);
	}
	system("sync");
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 27 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s radius configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	
	reply_status(conn, 200, "OK");
}


/***************************************************debug.html********************************************************************/
static void getdebug(struct mg_connection *conn, const struct mg_request_info *ri)
{
	
	char buf[255];
	memset(buf,0,sizeof(buf));
	FILE *file = NULL;
	char str[255];
	char line[255];
	int i;

	for(i=0;i<19;i++)
	{
		if(i==1)
		{
			file = NULL;
			memset(line,0,sizeof(line));
			file = popen("sed -n '31p' /clever/conf/device | cut -d= -f2", "r");
			if (file != NULL)
			{
				fgets(line, 32, file);
			}
			pclose(file);
			strcat(buf,line);
			strcat(buf,"?");
		}
		
		file = NULL;
		memset(str,0,sizeof(str));
		memset(line,0,sizeof(line));
		sprintf(str,"sed -n '%dp' /clever/conf/mac | cut -d= -f2", i+1);
		file = popen(str, "r");
		if (file != NULL)
		{
			fgets(line, 32, file);
		}
		pclose(file);
		strcat(buf,line);
		strcat(buf,"?");
	}

	mg_printf(conn,buf);
}


static void setdebug(struct mg_connection *conn, const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");

	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);	
	char *order7 = (char *)malloc(sizeof(char)*len);
	char *order8 = (char *)malloc(sizeof(char)*len);
	char *order9 = (char *)malloc(sizeof(char)*len);
	char *order10 = (char *)malloc(sizeof(char)*len);
	char *order11 = (char *)malloc(sizeof(char)*len);
	char *order12 = (char *)malloc(sizeof(char)*len);
	char *order13 = (char *)malloc(sizeof(char)*len);
	char *order14 = (char *)malloc(sizeof(char)*len);
	char *order15 = (char *)malloc(sizeof(char)*len);
	char *order16 = (char *)malloc(sizeof(char)*len);
	char *order17 = (char *)malloc(sizeof(char)*len);
	char *order18 = (char *)malloc(sizeof(char)*len);
	char *order19 = (char *)malloc(sizeof(char)*len);
	char *order20 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		memset(order8,0,sizeof(char)*len);
		req(request,order8,7);
		memset(order9,0,sizeof(char)*len);
		req(request,order9,8);
		memset(order10,0,sizeof(char)*len);
		req(request,order10,9);
		memset(order11,0,sizeof(char)*len);
		req(request,order11,10);
		memset(order12,0,sizeof(char)*len);
		req(request,order12,11);
		memset(order13,0,sizeof(char)*len);
		req(request,order13,12);
		memset(order14,0,sizeof(char)*len);
		req(request,order14,13);
		memset(order15,0,sizeof(char)*len);
		req(request,order15,14);
		memset(order16,0,sizeof(char)*len);
		req(request,order16,15);
		memset(order17,0,sizeof(char)*len);
		req(request,order17,16);
		memset(order18,0,sizeof(char)*len);
		req(request,order18,17);
		memset(order19,0,sizeof(char)*len);
		req(request,order19,18);
		memset(order20,0,sizeof(char)*len);
		req(request,order20,19);
        free(request);
	}

	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1cMAC=%s' /clever/conf/mac",order1);
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '31cdebug=%d' /clever/conf/device",atoi(order2));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '2cvh=%d' /clever/conf/mac",atoi(order3));
	system(order);
	system("sync");
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '3ctype=%d' /clever/conf/mac",atoi(order4));
	system(order);

	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '4cC1=%d' /clever/conf/mac",atoi(order5));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '5cC2=%d' /clever/conf/mac",atoi(order6));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '6cC3=%d' /clever/conf/mac",atoi(order7));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '7cC4=%d' /clever/conf/mac",atoi(order8));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '8cC5=%d' /clever/conf/mac",atoi(order9));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '9cC6=%d' /clever/conf/mac",atoi(order10));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '10cd1=%d' /clever/conf/mac",atoi(order11));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '11cd2=%d' /clever/conf/mac",atoi(order12));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '12cd3=%d' /clever/conf/mac",atoi(order13));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '13cd4=%d' /clever/conf/mac",atoi(order14));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '14cd5=%d' /clever/conf/mac",atoi(order15));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '15cd6=%d' /clever/conf/mac",atoi(order16));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '16cL1=%d' /clever/conf/mac",atoi(order17));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '17cL2=%d' /clever/conf/mac",atoi(order18));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '18cL3=%d' /clever/conf/mac",atoi(order19));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '19cee=%d' /clever/conf/mac",atoi(order20));
	system(order);
	
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	free(order7);
	free(order8);
	free(order9);
	free(order10);
	free(order11);
	free(order12);
	free(order13);
	free(order14);
	free(order15);
	free(order16);
	free(order17);
	free(order18);
	free(order19);
	free(order20);

	system("sync");
	reply_status(conn, 200, "OK");
}


/**/
static void gethw(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("gethw\n");
	
	char buf[255];
	memset(buf,0,sizeof(buf));
	FILE *file = NULL;
	char str[255];
	char line[255];
	int n;

	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '1p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	memcpy(shm->partNumber, line, sizeof(line));
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '2p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	shm->hardwareVersion = line[0];

	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '3p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	shm->modbusVersion = atoi(line);
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '4p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	n = atoi(line);
	if(n == 220)
	{
		strcat(buf,"0");
		strcat(buf,"?");
	}
	else if(n == 380)
	{
		strcat(buf,"1");
		strcat(buf,"?");
	}
//	shm->retVol[0] = n;
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '5p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	n = atoi(line);
	if(n == 16)
	{
		strcat(buf,"0");
		strcat(buf,"?");
	}
	else if(n == 32)
	{
		strcat(buf,"1");
		strcat(buf,"?");
	}
	else if(n == 63)
	{
		strcat(buf,"2");
		strcat(buf,"?");
	}
//	shm->retCurIn[0] = n;
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '6p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	shm->retfreq[0] = atoi(line);
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '7p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	shm->phaseNumber[0] = atoi(line);
	
	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '8p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	n = atoi(line);
	if(n == 16)
	{
		strcat(buf,"0");
		strcat(buf,"?");
	}
	else if(n == 20)
	{
		strcat(buf,"1");
		strcat(buf,"?");
	}

	file = NULL;
	memset(line,0,sizeof(line));
	file = popen("sed -n '9p' /clever/conf/hw | cut -d= -f2", "r");
	if (file != NULL)
	{
		fgets(line, 32, file);
	}
	pclose(file);
	strcat(buf,line);
	strcat(buf,"?");
//	shm->breakType[0] = n*10;
	
	printf("%s\n",buf);
	mg_printf(conn,buf);
}


static void sethw(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("sethw\n");
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	printf("%s\n", ri->query_string);
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);	
	char *order7 = (char *)malloc(sizeof(char)*len);
	char *order8 = (char *)malloc(sizeof(char)*len);
	char *order9 = (char *)malloc(sizeof(char)*len);

	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		memset(order7,0,sizeof(char)*len);
		req(request,order7,6);
		memset(order8,0,sizeof(char)*len);
		req(request,order8,7);
		memset(order9,0,sizeof(char)*len);
		req(request,order9,8);
        free(request);
	}

	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1cpartNumber=%s' /clever/conf/hw",order1);
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '2chardwareVersion=%s' /clever/conf/hw",order2);
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '3cmodbusVersion=%s' /clever/conf/hw",order3);
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '4cretVol=%d' /clever/conf/hw",atoi(order4));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '5cretCurIn=%d' /clever/conf/hw",atoi(order5));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '6cretfreq=%d' /clever/conf/hw",atoi(order6));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '7cphaseNumber=%d' /clever/conf/hw",atoi(order7));
	system(order);
	
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '8cbreakType=%d' /clever/conf/hw",atoi(order8));
	system(order);

	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '9cleakageFlag=%d' /clever/conf/hw",atoi(order9));
	system(order);
	
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	free(order7);
	free(order8);
	free(order9);

	system("sync");
	reply_status(conn, 200, "OK");
}

/**/
static void getratedoutput(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("getratedoutput\n");
	
	char buf[255];
	memset(buf,0,sizeof(buf));
	char str[255];
	int i;

	for(i=0; i<48; i++)
	{
		memset(str,0,sizeof(str));
		if(shm->retCurOut[0][i] == 100)
			sprintf(str,"%d",0);
		else
			sprintf(str,"%d",1);
		strcat(buf,str);
		strcat(buf,"?");
	}
	
	printf("%s\n",buf);
	mg_printf(conn,buf);
}


static void setratedoutput(struct mg_connection *conn, const struct mg_request_info *ri)
{
	printf("setratedoutput\n");
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	printf("%s\n", ri->query_string);
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	int i;
	char *order[48];
	for(i = 0; i < 48; i++)
	{
		order[i] = (char *)malloc(sizeof(char)*len);
	}
	
	if(request)
	{
		for(i = 0; i < 48; i++)
		{
			memset(order[i],0,sizeof(char)*len);
			req(request,order[i],i);
		}
		
        free(request);
	}

	char line[255];
	for(i=0; i<48; i++)
	{
		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '%dcratedoutput=%s' /clever/conf/ratedoutput", i+1, order[i]);
		system(line);
		shm->retCurOut[0][i]=atoi(order[i]);
	}

	
	for(i = 0; i < 48; i++)
	{
		free(order[i]);
	}

	system("sync");
	reply_status(conn, 200, "OK");
}

static void adjust(struct mg_connection *conn,  const struct mg_request_info *ri)
{
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
	}
	int index = atoi(order1);
	free(order1);
	
	char setbuf[255];
	memset(setbuf,0,sizeof(char)*255);
	
	shm->adjustmodel=1;
	switch(index)
	{
		case 0:
			shm->adjustmodel=1;
			break;
		case 1:
			sprintf(setbuf,"/clever/bin/order 0 1 0 5");
			system(setbuf);
			break;
		case 2:
			sprintf(setbuf,"/clever/bin/order 0 2 0 5");
			system(setbuf);
			break;
		case 3:
			sprintf(setbuf,"/clever/bin/order 0 2 0 5");
			system(setbuf);
			break;
	}
	
	reply_status(conn, 200, "OK");
}

/***************************************************energylog.html********************************************************************/
static void getcomenergy(struct mg_connection *conn, const struct mg_request_info *ri)
{

	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
	}
	free(request);
	
	int i;
	int gpflag=-1;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && onlineper[i].ip!=0)
		{
			gpflag = i;
			break;
		}
	}
	if(gpflag>9 || gpflag<0)
	{
		return;
	}
	
	char start[255];
	memset(start,0,sizeof(char)*255);
	sprintf(start,"/clever/log/logenergy/%s",order1);
	char end[255];
	memset(end,0,sizeof(char)*255);
	sprintf(end,"/clever/log/logenergy/%s",order2);
	free(order1);
	free(order2);

	int fd1;
	fd1=open(start,O_RDONLY,0666);
	if(fd1<0)
	{
		mg_printf(conn, "err1");
		return;
	}
	struct logkwh s_logkwh;
	memset(&s_logkwh,0,sizeof(struct logkwh));
	read(fd1,&s_logkwh,sizeof(struct logkwh));
	close(fd1);
	int fd2;
	fd2=open(end,O_RDONLY,0666);
	if(fd2<0)
	{
		mg_printf(conn, "err1");
		return;
	}
	struct logkwh e_logkwh;
	memset(&e_logkwh,0,sizeof(struct logkwh));
	read(fd2,&e_logkwh,sizeof(struct logkwh));
	close(fd2);
	char buf[2048];
	memset(buf,0,sizeof(char)*2048);
	char rebuf[255];
	memset(rebuf,0,sizeof(char)*255);
	sprintf(rebuf,"%d?%d?",shm->outputnum[0],shm->type[0]);
	strcat(buf,rebuf);
	
	
	if(shm->type[0]%4==2 || shm->type[0]%4==0)
	{
		if(shm->gourpflag[0]==0)
		{
			for(i=0;i<42;i++)
			{
				if(onlineper[gpflag].gpindex)
				{
					if(gp[onlineper[gpflag].gpindex].outputper[0][i]==0)
					{
						strcat(buf,"0?0?0?0?");
						continue;
					}
				}
			
				memset(rebuf,0,sizeof(rebuf));
				sprintf(rebuf,"%d?%s?%.1f?%.1f?",shm->outputexist[0][i],shm->outputname[0][i],(float)((float)e_logkwh.kwh[i]/10),(float)((float)s_logkwh.kwh[i]/10));
				strcat(buf,rebuf);
			}

			for(i=0;i<8;i++)
			{
				strcat(buf,"0?0?0?0?");
			}
		}
		else if(shm->gourpflag[0]==1)
		{
			int flag_1[10][42]={0};
			int flag_2[8]={0};
			int num=1;
			int j,n;
			
			for(j=0;j<8;j++)
			{
				for(n=0;n<42;n++)
				{
					if(shm->group[0][j][n]==1)
					{
						flag_1[0][n]=1;
						flag_2[j]=1;
					}
				}
			}
			
			for(i=0;i<42;i++)
			{
				if(flag_1[0][i]==1)
				{
					strcat(buf,"0?0?0?0?");
				}
				else
				{
					if(onlineper[gpflag].gpindex)
					{
						if(gp[onlineper[gpflag].gpindex].outputper[0][i]==0)
						{
							strcat(buf,"0?0?0?0?");
							continue;
						}
					}
					if(shm->outputexist[0][i]>0)
					{	
						memset(rebuf,0,sizeof(rebuf));
						sprintf(rebuf,"%d?%s?%.1f?%.1f?",shm->outputexist[0][i],shm->outputname[0][i],(float)((float)e_logkwh.kwh[i]/10),(float)((float)s_logkwh.kwh[i]/10));
						strcat(buf,rebuf);
						
						num++;
					}
					else
					{
						strcat(buf,"0?0?0?0?");
					}
				}
			}

			float S_kwh; 
			float E_kwh;
			
			for(i=0;i<8;i++)
			{
				S_kwh = 0.0; 
				E_kwh = 0.0;
				
				if(flag_2[i]==1)
				{
					for(n=0;n<42;n++)
					{
						if(shm->group[0][i][n]==1)
						{
							E_kwh = E_kwh + (float)((float)e_logkwh.kwh[n]/10);
						}
					}
					
					for(n=0;n<42;n++)
					{
						if(shm->group[0][i][n]==1)
						{
							S_kwh = S_kwh + (float)((float)s_logkwh.kwh[n]/10);
						}
					}
					
					memset(rebuf,0,sizeof(char)*255);
					sprintf(rebuf,"%d?%s?%.1f?%.1f?",num,shm->groupname[0][i],S_kwh,E_kwh);
					strcat(buf,rebuf);
					
					num++;
				}
				else
				{
					strcat(buf,"0?0?0?0?");
				}
			}
		}
	}
	
	if(shm->type[0]==1 || shm->type[0]==3 || shm->type[0]==9 || shm->type[0]==11 || shm->type[0]==17 || shm->type[0]==18 || shm->type[0]==19 || shm->type[0]==20)
	{
		memset(rebuf,0,sizeof(char)*255);
		sprintf(rebuf,"1?L1?%.1f?%.1f?",(float)((float)s_logkwh.totalkwh[0]/10),(float)((float)e_logkwh.totalkwh[0]/10));
		strcat(buf,rebuf);
	}
	else if(shm->type[0]==5 || shm->type[0]==7 || shm->type[0]==13 || shm->type[0]==15)
	{
		memset(rebuf,0,sizeof(char)*255);
		sprintf(rebuf,"1?L1?%.1f?%.1f?",(float)((float)s_logkwh.totalkwh[0]/10),(float)((float)e_logkwh.totalkwh[0]/10));
		strcat(buf,rebuf);

		memset(rebuf,0,sizeof(char)*255);
		sprintf(rebuf,"2?L2?%.1f?%.1f?",(float)((float)s_logkwh.totalkwh[1]/10),(float)((float)e_logkwh.totalkwh[1]/10));
		strcat(buf,rebuf);
		
		memset(rebuf,0,sizeof(char)*255);
		sprintf(rebuf,"3?L3?%.1f?%.1f?",(float)((float)s_logkwh.totalkwh[2]/10),(float)((float)e_logkwh.totalkwh[2]/10));
		strcat(buf,rebuf);
	}

	
	
	mg_printf(conn,buf);
}

static void getsettings(struct mg_connection *conn,
		const struct mg_request_info *ri) {

	system("tar -cf /clever/bin/www/system/settings.tar /clever/npminfo /clever/userinfo /clever/ginfo /clever/conf/device /clever/conf/snmpd.conf ");
	char dbinfo[255];
	memset(dbinfo,0,sizeof(char)*255);
	sprintf(dbinfo,"/clever/bin/insertdb 1 %s 3 1","getsettings");
	system(dbinfo);
	mg_printf(conn, "HTTP/1.1 302 Found\r\n"
			"Set-Cookie: original_url=%s\r\n"
			"Location: %s\r\n\r\n",
			"/", "/system/settings.tar");
}

#define ZEBRA_CODE_LEN		6
#define LINUX_DOMAIN "/tmp/web.domain"
static void getzebracode(struct mg_connection *conn, const struct mg_request_info *ri)
{
	int connect_fd = -1;
    int ret        = -1;
    int len        = 0;
    char rev_buf[64];
	char zebra_code[32];
	static struct sockaddr_un srv_addr;

    connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
    if(connect_fd < 0){
        printf("cannot create communication socket");
        goto end;
    }
    srv_addr.sun_family=AF_UNIX;
    strcpy(srv_addr.sun_path,LINUX_DOMAIN);
    ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(-1 == ret){
        perror("cannot connect to the zebra server");
  		goto end;
    }

    memset(rev_buf,0, sizeof(rev_buf));
    write(connect_fd, "getzebracode", 12);
    len = read(connect_fd, rev_buf, sizeof(rev_buf));
    if(ZEBRA_CODE_LEN == len){
    	printf("zebra code is: %s\n", rev_buf);
    	mg_printf(conn, rev_buf);
    }
    else{
    	printf("zebra code fail. lenght = %d\n", len);
    	mg_printf(conn, "err1");
    }

end:
	if(-1 != connect_fd)	close(connect_fd);
	return ;
}

#define BROADCAST_PORT_NUM         20100
int issafe_ch = 0;
static int write_zebrapara(int com_type, char *index_ip, int auto_status)
{
	int ret = -1;
	ret = iniFileLoad(CONF_FILE);

	if(-1 == ret){
		printf("ini file load fail.\n");
		return -1;
	}

	iniSetString("zebra", "index_ip", index_ip);
	//iniSetInt("com", "com_type", com_type, 0);   //通道选择由clever manager设置
	iniSetInt("zebra", "manual_setip", auto_status, 0);
	iniFileFree();
	system("sync");

#if 0
	shm->comType = com_type;
	if((CH_ZEBRA==com_type || CH_ZEBRA_SNMP==com_type || CH_ZEBRA_TCP==com_type) && 1!=issafe_ch){
		issafe_ch = 1;
		printf("start safe rules\n");
		system("/etc/nftables/nft_safe.sh");
	}
	else{
		if(2 != issafe_ch){
			issafe_ch = 2;
			printf("start unsafe rules\n");
			system("/etc/nftables/nft_unsafe.sh");
		}
	}
#endif
	return 0;
}

// this function was deprecated when set parameters using the Clever-Manager
static void setzebrapara(struct mg_connection *conn, const struct mg_request_info *ri)
{
	return ;
	int ret = -1;
	char *com_ch = NULL;
	char *index_ip = NULL;
	char *request = NULL;
	char *auto_status = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		mg_printf(conn, "err1");
		return;
	}
    request = decodeRequest(request);
	int len = strlen(request);
	com_ch = (char *)malloc(sizeof(char)*len);
	index_ip = (char *)malloc(sizeof(char)*len);
	auto_status = (char *)malloc(sizeof(char)*len);
	if(NULL == com_ch || NULL == index_ip || NULL == auto_status){
		if(com_ch){
			free(com_ch);
		}
		if(index_ip){
			free(index_ip);
		}
		if(auto_status)
		{
			free(auto_status);
		}
		mg_printf(conn, "err1");
		return ;
	}
	if(request)
	{
		memset(com_ch,0,sizeof(char)*len);
		req(request,com_ch,0);
		memset(index_ip,0,sizeof(char)*len);
		req(request, index_ip,1);
		memset(auto_status, 0, sizeof(char)*len);
		req(request,auto_status,2);
        free(request);
	}
	int channel = atoi(com_ch);
	int autoswitch = atoi(auto_status);

	ret = write_zebrapara(channel, index_ip, autoswitch);
	if(0 == ret){
		mg_printf(conn, "success");
	}
	else{
		mg_printf(conn, "err1");
	}
	free(com_ch);
	free(index_ip);
	free(auto_status);
}


static char *strrstr(const char *s1, const char *s2,int len)
{
	int len2;
	char *ps1;

	if (!(len2 = strlen(s2))) {
		return (char *)s1;
	}

	ps1 = (char *)s1 + len - 1;
	ps1 = ps1 - len2 + 1;

	while (ps1 >= s1) {
		if ((*ps1 == *s2) && (strncmp(ps1, s2, len2) == 0)) {
			return (char *)ps1;
		}
		ps1--;
	}

	return NULL;
}

static void upload(struct mg_connection *conn, const struct mg_request_info *ri){

	const char *header;
	char *pos_start,*pos_end;
	char *fp;
	if(checkuserper(ri->remote_ip,3))
	{
		mg_printf(conn, "No permission!");
		return;
	}
	FILE *fps =fopen("/clever.tar","wb+");
	if(fps==NULL){
		mg_printf(conn,"upload failed.\r\n\r\n");
		return;
	}
	int len=0;
	header=mg_get_header(conn,"Content-Length");

	len=atoi(header);
	if(len<=0){
		fclose(fps);
		return;

	}

	fp=(char *)malloc(len);
	if(fp==NULL){
		mg_printf(conn,"upload failed.\r\n\r\n");
		fclose(fps);
		return;
	}
	mg_read(conn,fp,len);
	char begin[]={0x0D,0x0A,0x0D,0x0A,0};
	char end[]={0x0D,0x0A,0x2D,0x2D,0};
	pos_start=strstr(fp,begin);
	if(pos_start==NULL){
		mg_printf(conn,"upload failed.\r\n\r\n");
		fclose(fps);
		return;
	}
	pos_end=strrstr(fp,end,len);
	if(pos_end==NULL){
		mg_printf(conn,"upload failed.\r\n\r\n");
		fclose(fps);
		return;
	}else if(pos_start == (pos_end-2)){
		mg_printf(conn,"empty file.\r\n\r\n");
		fclose(fps);
		return;
	}
	if(pos_end < pos_start){
		mg_printf(conn,"upload failed.\r\n\r\n");
		fclose(fps);
		return;

	}
	fwrite(pos_start+4,sizeof(char),pos_end-pos_start-4,fps);

	fclose(fps);
	system("/bin/busybox tar -xvf /clever.tar -C /");
	sleep(2);
	system("busybox rm -f /clever.tar");
	mg_printf(conn,"OK.\r\n\r\n");
	system("sync");
	system("reboot -f");

}



int ftp_add_user_passwd(char *nm,char *psd)
{
	if(nm==NULL || psd==NULL)
	{
		return -1;
	}
	char adduser[255];
	memset(adduser,0,sizeof(char)*255);

	//ftp acount name must different from sshd
	FILE *file;
	int len;
	char line[255];
	memset(line,0,sizeof(char)*255);
	//get sshd account name
	file = popen("sed -n '33p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return -1;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;


	//when the name is "", "root" "nobody" "sshd(used by ssh)"  line was sshd acount return
	if(nm[0]=='\0' || !strcmp(nm,"root") || !strcmp(nm,"nobody") || !strcmp(nm,"sshd") || !strcmp(nm,line))
	{
		return -1;
	}
	else
	{

		//before add user,we delete the preuser
		//FILE *file = NULL;
		//char line[255];
		//int len;
		memset(line,0,sizeof(char)*255);
		//get ftp prename                                                
		file = popen("sed -n '37p' /clever/conf/device | cut -d= -f2", "r");
		if(file!= NULL)
		{
			if (fgets(line, 255, file) == NULL)
			{
				pclose(file);
				return -1;
			}
		}
		pclose(file);
		len = strlen(line);
		if(len)
		{
			line[len-1]='\0';
		}
		//delete the preuser
		if(line[0] !='\0')
		{
			memset(adduser,0,sizeof(char)*255);
			sprintf(adduser,"deluser %s",line);
			system(adduser);
			//delete the prenm's home directory 		     
			memset(adduser,0,sizeof(char)*255);
			sprintf(adduser,"rm /home/%s -r",line);
			system(adduser);
		}

		//add user name=nm,  no input pasword
		memset(adduser,0,sizeof(char)*255);
		sprintf(adduser,"adduser  -D %s",nm);
		system(adduser);
		memset(adduser,0,sizeof(char)*255);
		//change passwd 
		sprintf(adduser,"(echo %s; sleep 1;echo %s) | passwd %s",psd,psd,nm);
		system(adduser);


	}
	return 0;
}


void initftp()
{
	char nm[255];
	char psd[255];
	char prenm[255];
	char prepsd[255];
	char line[255];
	memset(nm,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	memset(prenm,0,sizeof(char)*255);
	memset(prepsd,0,sizeof(char)*255);
	char adduser[255];
	memset(adduser,0,sizeof(char)*255);
	//get ftp name
	FILE *file;
	int len;
	file = popen("sed -n '37p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(nm,line,sizeof(char)*255);
	//get ftp password
	file = popen("sed -n '38p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(psd,line,sizeof(char)*255);

	file = popen("sed -n '1p' /clever/conf/usercopy | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(prenm,line,sizeof(char)*255);

	file = popen("sed -n '2p' /clever/conf/usercopy | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	file = NULL;
	strncpy(prepsd,line,sizeof(char)*255);

	//when the name is "", "root" "nobody" "sshd" jump adduser
	if(nm[0]=='\0' || !strcmp(nm,"root")  || !strcmp(nm,"nobody")  || !strcmp(nm,"sshd") || (!strcmp(nm,prenm) && !strcmp(psd,prepsd)))
	{
		return;
	}
	//add user name=nm, password=psd and delet the prename
	else
	{
		if(prenm[0] !='\0')
		{
			memset(adduser,0,sizeof(char)*255);
			sprintf(adduser,"deluser %s",prenm);
			system(adduser);
			//delete the prenm's home directory 		     
			memset(adduser,0,sizeof(char)*255);
			sprintf(adduser,"rm /home/%s -rf",prenm);
			system(adduser);
		}
		memset(adduser,0,sizeof(char)*255);
		sprintf(adduser,"adduser  -D %s",nm);
		system(adduser);
		memset(adduser,0,sizeof(char)*255);
		//change passwd 
		sprintf(adduser,"(echo %s; sleep 1;echo %s) | passwd %s",psd,psd,nm);
		system(adduser);

		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '1cftpuser=%s' /clever/conf/usercopy",nm);
		system(line);
		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '2cftppsd=%s' /clever/conf/usercopy",psd);
		system(line);

		//setting vsftpd config, let only the nm can login.
		memset(line,0,sizeof(char)*255);
		sprintf(line,"sed -i '1c%s' /etc/ftpusers",nm);
		system(line);

		system("sync");
	}

}


static void getftp(struct mg_connection *conn,
		const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	memset(line,0,sizeof(char)*255);
	//is ftp enable?
	file = popen("sed -n '36p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ftp name
	file = popen("sed -n '37p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ftp password
	file = popen("sed -n '38p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get ftp port number 21
	file = popen("sed -n '39p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	mg_printf(conn,buf);
}
static void setftp(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		free(request);
	}
	int ftpen=atoi(order1);
	int port=atoi(order4);
	char nm[255];
	char psd[255];
	memset(nm,0,sizeof(char)*255);
	memset(psd,0,sizeof(char)*255);
	strncpy(nm,order2,sizeof(char)*30);
	strncpy(psd,order3,sizeof(char)*30);
	free(order1);
	free(order2);
	free(order3);
	free(order4);
	if(ftpen<0 || ftpen>1)
	{
		mg_printf(conn, "err1");
		return;
	}
	if(port<0 || port>65535)
	{
		mg_printf(conn, "err1");
		return;
	}

	if(ftp_add_user_passwd(nm,psd) != 0)
	{
		mg_printf(conn, "err1");
		return;
	}

	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '36cftpen=%d' /clever/conf/device",ftpen==0?1:0);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '37cftpuser=%s' /clever/conf/device",nm);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '38cftppsd=%s' /clever/conf/device",psd);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '39cftpp=%d' /clever/conf/device",port);
	system(order);

	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1cftpuser=%s' /clever/conf/usercopy",nm);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '2cftppsd=%s' /clever/conf/usercopy",psd);
	system(order);

	//setting vsftpd config, let only the nm can login.
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '1c%s' /etc/ftpusers",nm);
	system(order);

	system("sync");
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 20 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s ftp configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}
	
	reply_status(conn, 200, "OK");
}


static void getmodbus(struct mg_connection *conn, const struct mg_request_info *ri)
{
	FILE *file = NULL;
	char line[255];
	char buf[255];
	memset(buf,0,sizeof(char)*255);
	memset(line,0,sizeof(char)*255);
	//modbus addr
	file = popen("sed -n '40p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	int len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get baud rate
	file = popen("sed -n '41p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get modbus data bits
	file = popen("sed -n '42p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get modbus parity
	file = popen("sed -n '43p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get modbus stopbit
	file = popen("sed -n '44p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	//get modbus TCP port
	file = popen("sed -n '45p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			mg_printf(conn, "err1");
			pclose(file);
			return;
		}
	}
	pclose(file);
	len = strlen(line);
	if(len)
	{
		line[len-1]='\0';
	}
	strcat(buf,line);
	strcat(buf,"?");
	file = NULL;
	mg_printf(conn,buf);
}

static void setmodbus(struct mg_connection *conn, const struct mg_request_info *ri)
{
	if(checkuserper(ri->remote_ip,1))
	{
		mg_printf(conn, "flase");
		return;
	}
	char *request = NULL;
	request = getrequest(ri->query_string,0,"?");
	if(!request)
	{
		return;
	}
	request = decodeRequest(request);
	int len = strlen(request);
	char *order1 = (char *)malloc(sizeof(char)*len);
	char *order2 = (char *)malloc(sizeof(char)*len);
	char *order3 = (char *)malloc(sizeof(char)*len);
	char *order4 = (char *)malloc(sizeof(char)*len);
	char *order5 = (char *)malloc(sizeof(char)*len);
	char *order6 = (char *)malloc(sizeof(char)*len);
	if(request)
	{
		memset(order1,0,sizeof(char)*len);
		req(request,order1,0);
		memset(order2,0,sizeof(char)*len);
		req(request,order2,1);
		memset(order3,0,sizeof(char)*len);
		req(request,order3,2);
		memset(order4,0,sizeof(char)*len);
		req(request,order4,3);
		memset(order5,0,sizeof(char)*len);
		req(request,order5,4);
		memset(order6,0,sizeof(char)*len);
		req(request,order6,5);
		free(request);
	}
	int addr=atoi(order1);
	int dbit=atoi(order3);


	if(addr<=0 || addr>255)
	{
		mg_printf(conn, "err1");
		return;
	}
	if(dbit< 6 || dbit > 8)
	{
		mg_printf(conn, "err1");
		return;
	}



	char order[255];
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '40caddr=%s' /clever/conf/device",order1);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '41cbaud=%s' /clever/conf/device",order2);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '42cdatabits=%s' /clever/conf/device",order3);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '43cparity=%s' /clever/conf/device",order4);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '44cstopbits=%s' /clever/conf/device",order5);
	system(order);
	memset(order,0,sizeof(char)*255);
	sprintf(order,"sed -i '45cstopbits=%s' /clever/conf/device",order6);
	system(order);

	system("sync");

	free(order1);
	free(order2);
	free(order3);
	free(order4);
	free(order5);
	free(order6);
	
	int i;
	for(i=0;i<10;i++)
	{
		if(onlineper[i].ip == ri->remote_ip && ri->remote_ip)
		{
			break;
		}
	}
	if(i>=0 && i<10)
	{
		char dbinfo[255];
		memset(dbinfo,0,sizeof(char)*255);
		sprintf(dbinfo,"/clever/bin/insertdb 24 %s 0 1",onlineper[i].name);
		system(dbinfo);
		
		char sys_info[255];
		memset(sys_info,0,sizeof(char)*255);
		sprintf(sys_info,"/clever/bin/syslog_send 4 'Network Settings:' '%s modbus configuration was changed.'",onlineper[i].name);
		system(sys_info);
	}

	reply_status(conn, 200, "OK");
}  



static const struct test_config 
{ 
	enum mg_event event; 
	const char *uri; 
	void (*func)(struct mg_connection *, const struct mg_request_info *);
} test_config[] = {
  {MG_NEW_REQUEST, "/check_register",	&check_register},
  {MG_NEW_REQUEST, "/user_register",	&user_register},
  {MG_NEW_REQUEST, "/user_login", 		&user_login},
  {MG_NEW_REQUEST, "/npmslavelist",		&npmslavelist},
  {MG_NEW_REQUEST, "/langflag",			&langflag},
  {MG_NEW_REQUEST, "/monitorMovement",	&monitorMovement},

  {MG_NEW_REQUEST, "/total",			&total},
  {MG_NEW_REQUEST, "/getoutput", 		&getoutput},

  {MG_NEW_REQUEST, "/getuserinfo",		&getuserinfo},
  {MG_NEW_REQUEST, "/getgroupinfo",		&getgroupinfo},
  {MG_NEW_REQUEST, "/userlistinfo", &userlistinfo},
  {MG_NEW_REQUEST, "/deleteuser", 	&deleteuser},
  {MG_NEW_REQUEST, "/adduser", 		&adduser},
  {MG_NEW_REQUEST, "/changeuser", 	&changeuser},
  {MG_NEW_REQUEST, "/gpmainper1", 	&gpmainper1},
  {MG_NEW_REQUEST, "/savegp",		&savegp},
  {MG_NEW_REQUEST, "/deletegp",		&deletegp},
  {MG_NEW_REQUEST, "/gpmainper2", 	&gpmainper2},
  {MG_NEW_REQUEST, "/savegpoutput", &savegpoutput},

  {MG_NEW_REQUEST, "/getmac", 		&getmac},
  {MG_NEW_REQUEST, "/getdata", 		&getdata},
  {MG_NEW_REQUEST, "/getdevice", 	&getdevice},
  {MG_NEW_REQUEST, "/setmode", 	&setmode},	
  {MG_NEW_REQUEST, "/setms", 		&setms},
  {MG_NEW_REQUEST, "/setname", 		&setname},
  {MG_NEW_REQUEST, "/getgroup", 	&getgroup},
  {MG_NEW_REQUEST, "/setgroup", 	&setgroup},
  {MG_NEW_REQUEST, "/getthreshold", &getthreshold},
  {MG_NEW_REQUEST, "/setthreshold", &setthreshold},
  {MG_NEW_REQUEST, "/setoverload",  &setoverload},
  {MG_NEW_REQUEST, "/getoverload",  &getoverload},
  {MG_NEW_REQUEST, "/settiming", 	&settiming},
  {MG_NEW_REQUEST, "/gettiming", 	&gettiming},
  {MG_NEW_REQUEST, "/setping",	 	&setping},
  {MG_NEW_REQUEST, "/getping", 		&getping},
  {MG_NEW_REQUEST, "/getsensor", 	&getsensor},
  {MG_NEW_REQUEST, "/setsensor", 	&setsensor},
  {MG_NEW_REQUEST, "/getswitch", 	&getswitch},
  {MG_NEW_REQUEST, "/setswitch", 	&setswitch},
  {MG_NEW_REQUEST, "/switchlock", 	&switchlock},
  {MG_NEW_REQUEST, "/getenergy", 	&getenergy},
  {MG_NEW_REQUEST, "/setenergy", 	&setenergy},
  {MG_NEW_REQUEST, "/geteth0info",  &geteth0info},
  {MG_NEW_REQUEST, "/setIPv4",  	&setIPv4},
  {MG_NEW_REQUEST, "/getwlan0info", &getwlan0info},
  {MG_NEW_REQUEST, "/searchwlan0",  &searchwlan0},
  {MG_NEW_REQUEST, "/setwlan0id", 	&setwlan0id},
  {MG_NEW_REQUEST, "/setwlan0info", &setwlan0info},
  {MG_NEW_REQUEST, "/getsnmpinfo",  &getsnmpinfo},
  {MG_NEW_REQUEST, "/setsnmp", 		&setsnmp},
  {MG_NEW_REQUEST, "/setsnmpv3", 	&setsnmpv3},
  {MG_NEW_REQUEST, "/getsmtpinfo",  &getsmtpinfo},
  {MG_NEW_REQUEST, "/setsmtp", 		&setsmtp},
  {MG_NEW_REQUEST, "/smtptest", 	&smtptest},
  {MG_NEW_REQUEST, "/getntpinfo",   &getntpinfo},
  {MG_NEW_REQUEST, "/ntpsync", 		&ntpsync},
  {MG_NEW_REQUEST, "/setntp", 		&setntp},
  {MG_NEW_REQUEST, "/settm", 		&settm},
  {MG_NEW_REQUEST, "/operatelog", 	&operatelog},
  {MG_NEW_REQUEST, "/alarmlog", 	&alarmlog},
  {MG_NEW_REQUEST, "/clearlogs", 	&clearlogs},
  {MG_NEW_REQUEST, "/clearalarmlogs", 	&clearalarmlogs},
  {MG_NEW_REQUEST, "/getsyslog", 	&getsyslog},
  {MG_NEW_REQUEST, "/setsyslog", 	&setsyslog},
  {MG_NEW_REQUEST, "/getweb", 		&getweb},
  {MG_NEW_REQUEST, "/setweb", 		&setweb},
  {MG_NEW_REQUEST, "/gettelnet", 	&gettelnet},
  {MG_NEW_REQUEST, "/settelnet", 	&settelnet},
  {MG_NEW_REQUEST, "/getradius", 	&getradius},
  {MG_NEW_REQUEST, "/setradius", 	&setradius},
  {MG_NEW_REQUEST, "/getdebug", 	&getdebug},
  {MG_NEW_REQUEST, "/setdebug", 	&setdebug},
  {MG_NEW_REQUEST, "/gethw", 		&gethw},
  {MG_NEW_REQUEST, "/sethw", 		&sethw},
  {MG_NEW_REQUEST, "/getratedoutput", 	&getratedoutput},
  {MG_NEW_REQUEST, "/setratedoutput", 	&setratedoutput},

  {MG_NEW_REQUEST, "/adjust", 		&adjust},
  {MG_NEW_REQUEST, "/getcomenergy", &getcomenergy},
  {MG_NEW_REQUEST, "/getsettings",  &getsettings},
  {MG_NEW_REQUEST, "/getssh", 		&getssh},
  {MG_NEW_REQUEST, "/setssh", 		&setssh},
  {MG_NEW_REQUEST, "/getftp", 		&getftp},
  {MG_NEW_REQUEST, "/setftp", 		&setftp},
  {MG_NEW_REQUEST, "/upload", 		&upload},
  {MG_NEW_REQUEST, "/getmodbus", 	&getmodbus},
  {MG_NEW_REQUEST, "/setmodbus", 	&setmodbus},
  {MG_NEW_REQUEST, "/getzebracode", &getzebracode},
  {MG_NEW_REQUEST, "/setzebrapara", &setzebrapara},
  {MG_NEW_REQUEST, "/systemset", 	&systemset},
  {0, NULL, NULL}
};

static void *callback(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info) 
{
	int i;
	for (i = 0; test_config[i].uri != NULL; i++) 
	{
		if (event == test_config[i].event && (event == MG_HTTP_ERROR || !strcmp(request_info->uri, test_config[i].uri))) 
		{
		  test_config[i].func(conn, request_info);
		  return "processed";
		}
	}
	return NULL;
}

int main(void) 
{
	
	printf("============================================web================================================\n");
	
	int port1,port2,ssl;
	FILE *file = NULL;
	char line[255];
	memset(line,0,sizeof(line));
	
	//获取http端口号
	file = popen("sed -n '2p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			printf("Get port1 err!");
		}
	}
	pclose(file);
	port1 = atoi(line);
	
	//获取https端口号
	file = NULL;
	file = popen("sed -n '21p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			printf("Get port2 err!");
		}
	}
	pclose(file);
	port2 = atoi(line);
	
	//获取ssl是否使能
	file = NULL;
	file = popen("sed -n '15p' /clever/conf/device | cut -d= -f2", "r");
	if(file!= NULL)
	{
		if (fgets(line, 255, file) == NULL)
		{
			printf("Get ssl err!");
		}
	}
	pclose(file);
	ssl = atoi(line); 
	

	char portstr[32];
	memset(portstr,0,sizeof(portstr));
    if(ssl)
	{
		sprintf(portstr,"%ds",port2);
	}
	else
	{
		sprintf(portstr,"%d",port1);
	}
	const char *options[] = {"document_root", "/clever/bin/www", "listening_ports", portstr, "ssl_certificate", "/clever/conf/ssl_cert.pem", NULL};

	if(initweb()!=0)
	{
	  return 0;
	}
	
	// initssh();
	// initftp();

	struct mg_context *ctx;
	ctx = mg_start(callback, NULL, options);

	int dayflag=0;

	pthread_t timer;//web登录后无操作计时
	pthread_create(&timer,NULL,&thread_timer,NULL);

	while(1)
	{
		time_t now;
		time(&now);
		struct tm *ct;
		ct = gmtime(&now);

		sleep(60);
		int day_my = ct->tm_mday;
		if(dayflag != day_my)
		{
			struct logkwh s_logkwh;
			int k;
			
			s_logkwh.totalkwh[0] = shm->Lkwh[0][0];
			s_logkwh.totalkwh[1] = shm->Lkwh[0][1];
			s_logkwh.totalkwh[2] = shm->Lkwh[0][2];
			for(k=0;k<42;k++)
			{
				s_logkwh.kwh[k] = shm->kwh[0][k];
			}
			
			char filename[255];
			memset(filename,0,sizeof(char)*255);
			sprintf(filename,"/clever/log/logenergy/%04d%02d%02d",ct->tm_year+1900,ct->tm_mon+1,ct->tm_mday);
			int fd;
			fd=open(filename,O_RDWR|O_CREAT,0666);
			lseek(fd,0,SEEK_SET);
			write(fd,&s_logkwh,sizeof(struct logkwh));
			close(fd);
			system("sync");
			dayflag = day_my;
		}
	}
	// delete_shm();
	// delete_sem();
  return 0;
}
