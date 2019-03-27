#include "headers.h"

int check_builtin()
{
	if(strcmp(eachcommand[0],"cd")==0 || strcmp(eachcommand[0],"pwd")==0 || strcmp(eachcommand[0],"echo")==0)
	{
		return 1;
	}
	return 0;
}

void builtin(int noarg)
{
	int i;
	if(strcmp(eachcommand[0],"cd")==0)
	{
		changedir(eachcommand[1]);
	}
	else if(strcmp(eachcommand[0],"pwd")==0)
	{
		char* retu  =(char*)malloc(100*sizeof(char));
		retu =getcwd(cwd,100);
		if(retu==NULL)
		{
			exit(0);
		}
		convertCWD(cwd);
		printf("%s\n",displayCWD);
	}
	else if(strcmp(eachcommand[0],"echo")==0)
	{
		//this is just general case
		for(i=1;i<noarg;i++)
		{
			printf("%s ",eachcommand[i]);
		}
		printf("\n");
	}
}

void changedir(char* dir)
{
	int ret;
	char* ans=(char*)malloc(100*sizeof(char));
	char* ch = (char*)malloc(sizeof(char));
	ch[0] = dir[0];
	if(ch[0] =='~')
	{
		strcpy(ans,home);
		strcat(ans,&dir[1]);
		strcpy(dir,ans);
	}
	else if(strcmp(dir,"-")==0)
	{
		strcpy(dir,prevdir);
	}
	getcwd(prevdir,100);
	ret = chdir(dir);
	if(ret == -1)
	{
		perror("Error ");
	}
	free(ans);
	free(ch);
}
