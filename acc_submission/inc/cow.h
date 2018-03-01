#ifndef COW_H
#define COW_H

#include "shm_util.h"


#define MAXROWSIZE 40
#define MAXCHARLEN 80

#define PRINTROWSIZE (2+MAXROWSIZE+2+1+1) //  spaces/slash + row content +  spaces/slash + newline + null

#define ROW1_BEGIN "/ "
#define ROW1_END " \\\n"
#define ROW2_BEGIN "\\ "
#define ROW2_END " /\n"

#define ROW1_BEGIN_LEN strlen(ROW1_BEGIN) 
#define ROW1_END_LEN strlen(ROW1_END) +1
#define ROW2_BEGIN_LEN strlen(ROW2_BEGIN) 
#define ROW2_END_LEN strlen(ROW2_END) +1

#define ROWS 9


class cow
{
	private:
		int semId;
		char *buf;
		FILE *fp;
		int mySem; // Semaphore indicating our user.
		int yourSem; // Semaphore indicating the other user
		shmUtil* shmutil;

	public:
		cow();
		int cowStart();
		int convertTextToCowSay(char *text, char (*output)[PRINTROWSIZE]);
		void printCowSay(char output[][PRINTROWSIZE]);


};

#endif
