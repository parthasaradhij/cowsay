#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cow.h"
#include "shm_util.h"

cow::cow()
{
	unsigned short vals[2]; // Values for initialising the semaphores.

	assert(1);
	fp = fopen("/tmp/cow.log", "w+");
	fp = stdout;
	shmutil = new shmUtil();

	shmutil->initShm(SHMKEY, SHMSIZE, false); 
	buf = shmutil->getBuf();

	vals[COW] = 0;
	vals[HUMAN] = 0;
	semId = shmutil->createSemaphores(2, vals,false);

	mySem = COW;
	yourSem = HUMAN;

	fprintf(fp, "semId %d, Unlocking human\n", semId);
	shmutil->semUnLock(semId, yourSem);
}
int cow::cowStart()
{


	int i = 0;
	while(1)
	{
		fprintf(fp, " iteration %d\n", ++i);

		// mySem is unlocked by hci 
		shmutil->semLock(semId, mySem);

		fprintf(fp,"INPUT %s\n", buf);
		convertTextToCowSay(buf, (char (*)[PRINTROWSIZE])buf+MAXCHARLEN);
		printCowSay((char (*)[PRINTROWSIZE])buf+MAXCHARLEN);
		fflush(fp);

		// Now unlock hci 
		shmutil->semUnLock(semId,yourSem);
	}

}

int main()
{
	cow *cowSayConverter = new cow();
	cowSayConverter->cowStart();
	return 0;
}

void cow::printCowSay(char output[][PRINTROWSIZE])
{
   int i = 0;
   for(i=0; i< ROWS; i++)
   {
     printf("%s", output[i]);
   }
}

int cow::convertTextToCowSay(char *text, char (*output)[PRINTROWSIZE])
{
	char tmpRow [MAXROWSIZE+1] = "                                        ";
	char emptyRow [MAXROWSIZE+1] = "                                        ";
	int offset = 0;


	fprintf(stdout, "Input string: %s\n", text);
	int len = strlen(text);
	if(len > MAXCHARLEN)
	{
		fprintf(stderr, "%s\n", "ERROR: Invalid length\n");
		return -1;
	}

	memcpy(output[0], "  ________________________________________  \n", PRINTROWSIZE);
	if(len > MAXROWSIZE)
	{
		// Fill row1
		offset = 0;
		memcpy(*(output+1), ROW1_BEGIN, ROW1_BEGIN_LEN);

		offset += ROW1_BEGIN_LEN;
		memcpy(*(output+1)+ offset, text, MAXROWSIZE);

		offset += MAXROWSIZE;
		memcpy(*(output+1)+offset, ROW1_END, ROW1_END_LEN);

		// Fill row2 with trailing spaces and end of line character
		offset = 0;
		memcpy(*(output+2), ROW2_BEGIN, ROW2_BEGIN_LEN);

		memcpy(tmpRow, text+MAXROWSIZE, len-MAXROWSIZE);

		offset += ROW2_BEGIN_LEN;
		memcpy(*(output+2)+offset,tmpRow, MAXROWSIZE);

		offset += MAXROWSIZE;
		memcpy(*(output+2)+offset, ROW2_END, ROW2_END_LEN);

	}
	else
	{
		// Fill row1 with trailing spaces and end of line character
		offset = 0;
		memcpy(*(output+1), ROW1_BEGIN, ROW1_BEGIN_LEN);
		
		memcpy(tmpRow, text, len);

		offset += ROW1_BEGIN_LEN;
		memcpy(*(output+1)+offset, tmpRow, MAXROWSIZE);


		offset += MAXROWSIZE;
		memcpy(*(output+1)+offset, ROW1_END, ROW1_END_LEN);

		// Fill empty row2 with end of line character
		offset = 0;
		memcpy(*(output+2),ROW2_BEGIN ,ROW2_BEGIN_LEN);

		offset += ROW2_BEGIN_LEN;
		memcpy(*(output+2)+offset,emptyRow, MAXROWSIZE);

		offset += MAXROWSIZE;
		memcpy(*(output+2)+offset, ROW2_END, ROW2_END_LEN);
	}
	memcpy(output[3],"  ----------------------------------------  \n", PRINTROWSIZE);
	memcpy(output[4],"       \\   ^__^                         \n", PRINTROWSIZE);
	memcpy(output[5],"        \\  (oo)\\_______                 \n",PRINTROWSIZE);
	memcpy(output[6],"           (__)\\       )\\/\\             \n", PRINTROWSIZE);
	memcpy(output[7],"               ||----w |                \n", PRINTROWSIZE);
	memcpy(output[8],"               ||     ||                \n", PRINTROWSIZE);

	return 0;
}

