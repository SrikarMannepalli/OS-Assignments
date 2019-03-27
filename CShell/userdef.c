#include "headers.h"

typedef struct jobs
{
	int id;
	char name[100];
	char status[100];
} jobs;

extern int prcount;
extern jobs jobs_list[100];


void ctrlz(int sig) 
{
	int i,flag = 0;
	if(shell_pid >0) 
	{
		kill(shell_pid,SIGTSTP);
		kill(shell_pid,SIGTTIN);
		for(i=0;i<prcount;i++)
		{
			if(jobs_list[i].id == shell_pid) 
			{
				strcpy(jobs_list[i].status,"STOPPED");
				flag = 1;
				break;
			}
		}
		if(flag == 0) 
		{
			jobs_list[prcount].id = shell_pid;
			strcpy(jobs_list[prcount].status,"STOPPED");
			strcpy(jobs_list[prcount].name,forename);
			prcount+=1;
		}	
	}
}

void jobsset()
{
	int status,i,j;
	pid_t ppid;
	for(i=0;i<prcount;i++)
	{
		ppid = waitpid(jobs_list[i].id,&status,WUNTRACED|WNOHANG);
		if(ppid>0)
		{
			if(WIFCONTINUED(status)!=0)
			{
				strcpy(jobs_list[i].status,"RUNNING");
			}
			else if(WIFSTOPPED(status)!=0)
			{
				strcpy(jobs_list[i].status,"STOPPED");
			}
			else if(WIFEXITED(status)!=0)
			{
				for(j=i;j<prcount;j++)
				{
					jobs_list[j] = jobs_list[j+1];
				}
				prcount-=1;
			}
			else if(WIFSIGNALED(status)!=0)
			{
				for(j=i;j<prcount;j++)
				{
					jobs_list[j] = jobs_list[j+1];
				}
				prcount-=1;
			}
		}
		else if(ppid <0)
		{
			for(j=i;j<prcount;j++)
			{
				jobs_list[j] = jobs_list[j+1];
			}
			prcount-=1;
		}
	}
} 

int check_user(int no_arg)
{
	if(strcmp(eachcommand[0],"quit")==0)
	{
		// kill(shell_pid,SIGKILL);
		printf("BYE!!\n");
		while(1) {
			_exit(0);
		}
	}
	else if(strcmp(eachcommand[0],"jobs")==0)
	{
		jobsset();
		jobsset();
		for(int i=0;i<prcount;i++)
		{
			printf("[%d] %s %s[%d]\n",i+1,jobs_list[i].status,jobs_list[i].name,jobs_list[i].id);
		}
		return 1;
	}
	else if(strcmp(eachcommand[0],"overkill")==0)
	{
		int i;
		jobsset();
		for(i=0;i<prcount;i++) 
		{
			kill(jobs_list[i].id,SIGKILL);
		}
		return 1;
	}
	else if(strcmp(eachcommand[0],"setenv")==0)
	{
		if(no_arg == 2)
		{
			if(setenv(eachcommand[1],"",1)<0) perror("Error");
		}
		else if(no_arg == 3)
		{
			if(setenv(eachcommand[1],eachcommand[2],1)<0) perror("Error");
		}
		else if(no_arg==1)
		{
			fprintf(stderr,"Min number of arguments is 2.\n");
		}
		else 
		{
			fprintf(stderr,"Too many arguments.\n");
		}
		return 1;
	}
	else if(strcmp(eachcommand[0],"getenv")==0) 
	{
		if(no_arg!=2) 
		{
			fprintf(stderr,"Usage: getenv [variable]");
		}
		else 
		{ 
			char* ret = (char*)malloc(100*sizeof(char));
			ret = getenv(eachcommand[1]);
			if(ret == NULL) perror("Error");
			else printf("%s\n",ret);
		}
		return 1;
	}
	else if(strcmp(eachcommand[0],"unsetenv")==0)
	{
		if(no_arg==1) 
		{
			fprintf(stderr,"Min number of arguments is 2.\n");
		} 
		else if(no_arg>2)
		{
			fprintf(stderr,"Too many arguments.\n");
		}
		else
		{
			if(unsetenv(eachcommand[1])<0) perror("Error");
		}
		return 1;
	}
	else if(strcmp(eachcommand[0],"fg")==0)
	{
		jobsset();
		int status,fg_no; 
		if(no_arg>2) 
		{
			fprintf(stderr,"Min number of arguments is 2.\n");
		}
		if(no_arg==1) fg_no = prcount;
		else fg_no = atoi(eachcommand[1]);
		kill(jobs_list[fg_no-1].id,SIGCONT);
		if(waitpid(jobs_list[fg_no-1].id,&status,WUNTRACED)<0) perror("Error");
		return 1;
	}
	else if(strcmp(eachcommand[0],"bg")==0)
	{
		jobsset();
		int status,bg_no; 
		if(no_arg>2) 
		{
			fprintf(stderr,"Min number of arguments is 2.\n");
		}
		if(no_arg==1) bg_no = prcount;
		else bg_no = atoi(eachcommand[1]);
		//
		int ret = kill(jobs_list[bg_no-1].id,SIGTTIN);
		if(ret==-1) perror("Error");
		ret = kill(jobs_list[bg_no-1].id,SIGCONT);
		if(ret==-1) perror("Error");
		strcpy(jobs_list[bg_no-1].status,"RUNNING");
		return 1;
	}
	else if(strcmp(eachcommand[0],"kjob")==0)
	{
		jobsset();
		if(no_arg!=3) 
		{
			fprintf(stderr,"Usage: kjob <jobNumber> <signalNumber>\n");
		}
		int jobNumber = atoi(eachcommand[1]);
		int signalNumber = atoi(eachcommand[2]);
		if(jobNumber<1 || jobNumber>prcount)
		{
			fprintf(stderr,"A job with the given job does not exist.\n");
		}
		else
		{
			printf("%d\n",jobs_list[jobNumber-1].id);
			int ret = kill(jobs_list[jobNumber-1].id,signalNumber);
			if(ret ==-1) perror("Error");
		}
		return 1;
	}
	return 0;
}
