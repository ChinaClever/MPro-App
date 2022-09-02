#ifndef CONFIG_SHM_H
#define CONFIG_SHM_H

/*
*	usage: 
*	ret = config_shm::get_instance()->write_config_shm(&cfg_args);
*	if(ret == FAIL)  ...
*	ret = config_shm::get_instance()->read_config_shm(&cfg_args);
*	if(ret == FAIL)  ...
*/
	

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#define  CONFIG_ARGS_NUM         	2
#define  CONFIG_KEY_FILE			"/"
#define  CONFIG_SHM_PROJ_ID			21
#define  CONFIG_SEM_PROJ_ID			22
#define  CONFIG_SEM_MEMBERS			1
#define  CONFIG_SEM_INITVAL			1
			
#define  SUC						0
#define  FAIL						-1

typedef struct _CONFIG_ARGS
{
	int com_type;	// 0-snmp, 1-zebra, 2-double
}CONFIG_ARGS;


union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *buf_info;
	void *pad;
};

CONFIG_ARGS *shm_init();
int write_config_shm(const CONFIG_ARGS* m_shm_ptr, const CONFIG_ARGS *cfg);
int read_config_shm(const CONFIG_ARGS* m_shm_ptr, CONFIG_ARGS *cfg);
int delete_shm();

int init_sem();
int order_sem_p();
int order_sem_v();
int delete_sem();


#endif
