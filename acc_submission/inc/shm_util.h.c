#ifndef SHM_UTIL_H
#define SHM_UTIL_H

#include <sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<stdbool.h>
#include<stdio.h>
#include<assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cowsay_util.h"

#define SHMSIZE ((MAXCHARLEN)+((ROWS)*(PRINTROWSIZE)))

#define SHMKEY 0x1233
#define SEMKEY 0x1234

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

enum
{
	human,
	cow
};


int initShm(int key, int size, char **pBuf, bool master);
int createSemaphores(int n, unsigned short* vals, bool master);
void semLock(int id, int i);
void semUnLock(int id, int i);


#endif


