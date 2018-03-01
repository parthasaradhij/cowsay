#include "shm_util.h"

shmUtil::shmUtil()
{
}
int shmUtil::initShm(int key, int size, bool master)
{
	shmFd = -1;
	assert(size>0);

	printf("key %x, size %d\n", key, size);
	if(master)
	{
		shmFd = shmget(key, size, IPC_CREAT | 0660);
	}
	else
	{
		shmFd = shmget(key, size,  0660);
	}
	printf("shmFd %d\n", shmFd);
   
	if(-1 == shmFd)
	{
		perror("shmget");
		return -1;
	}

	pBuf = (char*)shmat(shmFd, NULL, 0); /* Attach the segment... */
	printf("Buf addr %p", *pBuf);
	//shmctl(shmFd, IPC_RMID, NULL);
	return 0;
}

int shmUtil::createSemaphores(int n, unsigned short* vals, bool master)
{
	union semun arg;
	int id;

	assert(n > 0); 
	assert(vals != NULL); 

	if(master)
	{
		id = semget(0x1234, n, IPC_CREAT | 0666);
	}
	else
	{
		id = semget(0x1234, n, 0666);
	}
	
	arg.array = vals;
	semctl(id, 0, SETALL, arg);


	return id;
}

void shmUtil::semLock(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}

void shmUtil::semUnLock(int id, int i)
{
	struct sembuf sb;
	sb.sem_num = i;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	semop(id, &sb, 1);
}
char* shmUtil::getBuf()
{
	return pBuf;
}

