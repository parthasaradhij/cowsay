#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cowsay_util.h"

void printCowSay(char output[][PRINTROWSIZE])
{
   int i = 0;
   for(i=0; i< ROWS; i++)
   {
     printf("%s", output[i]);
   }
}

int convertTextToCowSay(char *text, char (*output)[PRINTROWSIZE])
{
	char tmpRow [MAXROWSIZE] = "                                        ";
	char emptyRow [MAXROWSIZE] = "                                        ";
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

