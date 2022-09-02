#include "config_shm.h"

int m_shm_id;
int m_sem_id;


CONFIG_ARGS *shm_init()
{
	CONFIG_ARGS* m_shm_ptr;

	int ret = init_sem();
    if(ret == FAIL)
	{
		printf("init sem failed!\n");
		return NULL;
	}

	key_t shm_key = ftok(CONFIG_KEY_FILE, CONFIG_SHM_PROJ_ID);
	if (shm_key == FAIL)
	{
		printf("ftok failed!\n");
		return NULL;
	}

    printf("ftok suc!\n");

	m_shm_id = shmget(shm_key, sizeof(CONFIG_ARGS) * CONFIG_ARGS_NUM, 0666 | IPC_CREAT);
	if (m_shm_id == FAIL)
    {
        printf("create zebra_shm failed!\n");
        return NULL;
    }
	printf("create zebra_shm suc!\n");

	m_shm_ptr = (CONFIG_ARGS*)shmat(m_shm_id, 0, 0);
	if (m_shm_ptr == (void*)-1)
    {
        printf("at zebra_shm failed!\n");
		return NULL;
    }
	printf("attach zebra_shm suc!\n");

	return m_shm_ptr;
}

int write_config_shm(const CONFIG_ARGS* m_shm_ptr, const CONFIG_ARGS *cfg)
{
	if (m_shm_ptr == NULL || m_shm_ptr == (CONFIG_ARGS*)-1)
	{
		printf( "m_shm_ptr is illegal.\n");
		return FAIL;
	}

	if(cfg == NULL)
	{
		printf( "cfg is null ptr\n");
		return FAIL;
	}

	int ret = FAIL;
	int find = 0;
	if ((ret = order_sem_p()) == FAIL)
	{
		printf( "order_sem_p() fail.\n");	
		return FAIL;
	}

	printf( "order_sem_p() suc.\n");

	memcpy(m_shm_ptr, cfg, sizeof(CONFIG_ARGS));

	printf( "write data finish\n");

	if ((ret = order_sem_v()) == FAIL)
	{
		printf( "order_sem_v() failed.\n");
		return FAIL;
	}
	printf( "order_sem_v() suc.\n");

	return SUC;
}

int read_config_shm(const CONFIG_ARGS* m_shm_ptr, CONFIG_ARGS *cfg)
{
	if (m_shm_ptr == NULL || m_shm_ptr == (CONFIG_ARGS*)-1)
	{
		printf( "m_shm_ptr is illegal.\n");
		return FAIL;
	}

	if(cfg == NULL)
	{
		printf( "cfg is null ptr\n");
		return FAIL;
	}

	int ret = FAIL;
	int find = 0;
	if ((ret = order_sem_p()) == FAIL)
	{
		printf( "order_sem_p() fail.\n");
		return FAIL;
	}

	printf( "order_sem_p() suc.\n");

	memcpy(cfg, m_shm_ptr, sizeof(CONFIG_ARGS));

	printf( "read data finish\n");

	if ((ret = order_sem_v()) == FAIL)
	{
		printf( "order_sem_v() failed.\n");
		return FAIL;
	}
	printf( "order_sem_v() suc.\n");

	return SUC;
}

/* delete zebra_shm*/
int delete_shm()
{
	int ret = shmctl(m_shm_id, IPC_RMID, 0);
	if (ret == FAIL)
	{
		printf( "delete_shm error!\n");
	}
	return ret;
}

/* create sem*/
int init_sem()
{
	key_t key_id;
	int index, sid;
	union semun semopts;

	if ((key_id = ftok(CONFIG_KEY_FILE, CONFIG_SEM_PROJ_ID)) == -1) // get key_id
	{
		printf( "ftok error!\n");
		return FAIL;
	}
	printf( "ftok suc!\n");

	if ((sid = semget(key_id, CONFIG_SEM_MEMBERS, IPC_CREAT | IPC_EXCL | 0666)) == -1) // get sem_id
	{
		printf( "sem exsit!\n");
        sid = semget(key_id, 0, 0666);
        if(sid == -1)
        {
            printf("sem get failed\n");
            return FAIL;
        }
        printf("sem get suc!\n");
	}
    else
    {
	    semopts.val = CONFIG_SEM_INITVAL; //init sem
	    semctl(sid, 0, SETVAL, semopts); 
	    printf( "sem create suc!\n");
    }
	printf( "sem create suc!\n");

	semopts.val = CONFIG_SEM_INITVAL; //init sem

	m_sem_id = sid;
    return SUC;
}

/* p operation*/
int order_sem_p()
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = -1;
	sem_arg.sem_flg = SEM_UNDO;

	if (semop(m_sem_id, &sem_arg, 1) == -1)
	{
		printf( "sem p error!\n");
		return FAIL;
	}
	return SUC;
}

/* v operation*/
int order_sem_v()
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = 1;
	sem_arg.sem_flg = SEM_UNDO;

	if (semop(m_sem_id, &sem_arg, 1) == -1)
	{
		printf( "sem v error!\n");
		return FAIL;
	}
	return SUC;
}

/* delete sem*/
int delete_sem()
{
	int ret = semctl(m_sem_id, 0, IPC_RMID);
	if (ret == FAIL)
	{
		printf( "delete_sem error!\n");
	}
	return ret;
}
