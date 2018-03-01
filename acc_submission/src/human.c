#include "shm_util.h"
#include "com_andium_cowsay_hci_CowSayConverter.h"

int cowsay_ready = 0;
int idShMem; // Shared memory handle.
unsigned short vals[2]; // Values for initialising the semaphores.
int mySem; // Semaphore indicating our user.
int yourSem; // Semaphore indicating the other user.
char *buf = NULL;
int semId = -1; 

char *input_arr[] = { "hello world, this is an example of cowsay!",
	  	      "another example",
	  	      "yet another example",
                      "ok folks, this is a cowsay using linux IPCs!"
		     };

shmUtil *shmutil;

int hci_init()
{
	shmutil = new shmUtil();
	shmutil->initShm(SHMKEY, SHMSIZE, true); 
	buf = shmutil->getBuf();

	vals[HUMAN] = 0;
	vals[COW] = 0;
	semId = shmutil->createSemaphores(2, vals, true);

	printf("SemId created %d", semId);

	mySem = HUMAN;
	yourSem = COW;

	printf("Waiting for cow to start\n");

	shmutil->semLock(semId, mySem);
	printf("Now text can be converted to cowsay\n");
	cowsay_ready = 1;

	return 0;
}



JNIEXPORT void JNICALL Java_com_andium_cowsay_hci_CowSayConverter_hci_1init
  (JNIEnv * env, jobject obj)
{
	hci_init();
}

//JNIEXPORT void JNICALL Java_com_andium_cowsay_hci_CowSayConverter_convert_1to_1cowsay

JNIEXPORT jobjectArray JNICALL Java_com_andium_cowsay_hci_CowSayConverter_convert_1to_1cowsay
  (JNIEnv * env, jobject obj, jstring string)
{
	jobjectArray ret;
	char (*output)[PRINTROWSIZE];

	const char *str = (*env).GetStringUTFChars(string, NULL);
	strcpy(buf,str);
	//FILE *fp = fopen("/tmp/output", "w"); 



	printf("input string(JNI): %s\n", buf);

	// Unlock cow's sem after updating the input in shm
	shmutil->semUnLock(semId,yourSem);
	// Lock hci's sem till the text is converted into cow langauge
	// Once converted, the sem would be unlocked by 'cow'
	shmutil->semLock(semId, mySem);
	output = (char (*)[PRINTROWSIZE])buf+MAXCHARLEN;
	//printCowSay((char (*)[PRINTROWSIZE])buf+MAXCHARLEN);

	//fflush(stdout);

	ret= (jobjectArray)(*env).NewObjectArray( ROWS,
			(*env).FindClass("java/lang/String"),
			(*env).NewStringUTF(""));

	for(int i=0;i<ROWS;i++) 
	{
		(*env).SetObjectArrayElement( ret,i,(*env).NewStringUTF(output[i]));
	}


	(*env).ReleaseStringUTFChars(string, str);

	return ret;
}


// The function below is only meant for UT
void input(char *buf)
{
	int ind = rand()%4;	
        strcpy(buf, input_arr[ind]);
}
void printCowsay(char output[][PRINTROWSIZE])
{
   int i = 0;
   for(i=0; i< ROWS; i++)
   {
     printf("%s", output[i]);
   }
}
// The function below is only meant for UT
int humanFunc()
{
	hci_init();

	while(1)
	{
		input(buf);
		//semUnLock(semId,mySem);
		shmutil->semUnLock(semId,yourSem);
		shmutil->semLock(semId, mySem);
		printCowsay((char (*)[PRINTROWSIZE])buf+80);
	}

}
/*
int main()
{

	humanFunc();
	return 0;
}
*/
