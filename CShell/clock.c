#include "headers.h"

int check_clock(int noarg)
{
	if(noarg!=5 && strcmp(eachcommand[0],"clock")==0)
	{
		printf("command format id clock -t seconds -n duration\n");
		return 0;
	}
	if(strcmp(eachcommand[0],"clock")==0)
	{
		if(strcmp(eachcommand[1],"-t")==0 && strcmp(eachcommand[3],"-n")==0)
		{
			return 1;
		}
		else
		{
			printf("no such flag.\n");
			return 0;
		}
	}
	return 0;
}

void clock1(char* timediffer,char* duration)
{
	int pid,fd,status;
	int tim  = atoi(timediffer);
	int dura  = atoi(duration);
	int timer = 0;
	pid  = fork();
	child_pid = pid;
	if(pid<0)
	{
		perror("Error");
		exit(0);
	}
	if(pid>0)
	{
		waitpid(pid,&status,0);
	}
	else
	{
		int count =0,ppid;
		char* ans;
		char* tok=(char*)malloc(1000*sizeof(char));
		char* buf = (char*)malloc(1000*sizeof(char));
		do
		{
			fd = open("/proc/driver/rtc",O_RDONLY);
			read(fd,buf,1000);
			tok = buf;
			ans = (char*)malloc(100*sizeof(char));

			while((tok = strtok(tok," :\t\n"))!=NULL)
			{
				if(strcmp(tok,"rtc_time")==0)
				{
					while(count<3)
					{
						count++;
						tok = NULL;
						tok = strtok(tok," :\t\n");
						if(count==1) strcpy(ans,tok);
						else strcat(ans,tok);
						if(count!=3) strcat(ans,":");
					}
					count =0;
					printf("%s\t",ans);
				}
				if(strcmp(tok,"rtc_date")==0)
				{
					tok = NULL;
					tok = strtok(tok," :\t\n");
					printf("%s\n",tok);
				}
				tok = NULL;
			}
			free(ans);
			close(fd);
			sleep(tim);
			timer+=tim;
		}while(timer<dura);
		free(buf);
		ppid = getpid();
	}
}
