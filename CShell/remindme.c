#include "headers.h"

int check_remindme(int noarg)
{
	if(noarg<2 && strcmp(eachcommand[0],"remindme")==0)
	{
		printf("command format is remindme time statement.\n");
		return 0;
	}
	if(strcmp(eachcommand[0],"remindme")==0)
	{
		return 1;
	}
	return 0;
}

void remindme(char* timediffer,char* statement,int noarg)
{
	int pid,fd,status,count=2,ppid;
	int tim  = atoi(timediffer);
	pid  = fork();
	if(pid<0)
	{
		perror("Error");
		exit(0);
	}
	if(pid>0)
	{
		waitpid(pid,&status,WNOHANG);
	}
	else
	{
		sleep(tim);
		printf("\nReminder:");
		while(count<noarg)
		{
			printf("%s ",eachcommand[count]);
			count++;
		}
		printf("\n");
		ppid= getpid();
		kill(ppid,SIGKILL);
	}
}
