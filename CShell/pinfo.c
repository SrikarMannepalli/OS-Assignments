#include "headers.h"

int check_pinfo(int noarg)
{
	for(int i=0;i<noarg;i++)
	{
		if(strcmp(eachcommand[i],">") == 0)
			return 0;
		else if(strcmp(eachcommand[i],"<") == 0)
			return 0;
		else if(strcmp(eachcommand[i],">>") == 0)
			return 0;
	}
	
	if(strcmp(eachcommand[0],"pinfo")==0) return 1;

	return 0;
}

void dopinfo(int noarg)
{
	int ret;
	char* st = (char*)malloc(100*sizeof(char));
	char* exe = (char*)malloc(100*sizeof(char));
	char* pd = (char*)malloc(100*sizeof(char));
	char* buf = (char*)malloc(1000*sizeof(char));
	char* buf1 = (char*)malloc(100*sizeof(char));
	char* tok=(char*)malloc(1000*sizeof(char));
	char* ppid=(char*)malloc(1000*sizeof(char));
	char* pmem=(char*)malloc(1000*sizeof(char));
	char* pproc=(char*)malloc(1000*sizeof(char));

	if(noarg>1)
	{
		strcpy(pd,eachcommand[1]);
		strcpy(st,"/proc/");
		strcat(st,pd);
		strcat(st,"/status");
		strcpy(exe,"/proc/");
		strcat(exe,pd);
		strcat(exe,"/exe");
	}
	else
	{
		int pid = getpid();
		sprintf(st,"/proc/%d/status",pid);
		sprintf(exe,"/proc/%d/exe",pid);
	}
	int fd = open(st,O_RDONLY);
	if(fd==-1)
	{
		perror("Error");
		//printf("Process with the given pid does not exist.\n");
		//return;
	}
	read(fd,buf,500);
	tok = buf;
	while((tok=strtok(tok," :\t\n"))!=NULL)
	{
		if(strcmp(tok,"Pid")==0)
		{
			tok = NULL;
			tok = strtok(tok," :\n\t");
			strcpy(ppid,tok);
		}
		if(strcmp(tok,"State")==0)
		{
			tok = NULL;
			tok = strtok(tok," :\n\t");
			strcpy(pproc,tok);
		}
		if(strcmp(tok,"VmSize")==0)
		{
			tok = NULL;
			tok = strtok(tok," :\n\t");
			strcpy(pmem,tok);
		}
		tok = NULL;
	}
	printf("Pid -- %s\n",ppid);
	printf("Process Status -- %s\n",pproc);
	printf("Virtual memory -- %s\n",pmem);
	close(fd);
	ret = readlink(exe,buf1,100);
	if(ret==-1)
	{
		perror("Error");
	}
	else
	{
		convertCWD(buf1);
		printf("Executable path -- %s\n",buf1);
	}

	free(st);
	free(exe);
	free(buf);
	free(buf1);
	free(pd);
	free(tok);
	free(ppid);
	free(pmem);
	free(pproc);
}
