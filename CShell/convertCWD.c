#include "headers.h"


void convertCWD(char* cwd)
{
	int len,i,j;
	char* temp =(char*)malloc(100*sizeof(char));
	len = strlen(cwd);
	if(strcmp(cwd,home) == 0)
	{
		strcpy(displayCWD,"~");
		strcpy(cwd,"~");
		return;
	}
	strcpy(temp,"~");
	j = 1;
	//if both are not same

	if(strncmp(cwd,home,strlen(home))==0)
	{
		strcat(temp,&cwd[strlen(home)]);
		strcpy(displayCWD,temp);
		strcpy(cwd,temp);
	}
	else
	{
		strcpy(displayCWD,cwd);
	}
	free(temp);
}
