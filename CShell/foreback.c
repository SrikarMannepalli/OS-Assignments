#include "headers.h"

typedef struct jobs
{
	int id;
	char name[100];
	char status[100];
} jobs;

int prcount;
jobs jobs_list[100];

int status;
void handler(int sig)
{
	pid_t pid;
	int status;
	pid = waitpid(-1,&status,WNOHANG);

	if(pid>0 && WIFEXITED(status)==1)
	{
		bg_count-=1;
		fprintf(stderr,"\nProcess with pid %d exited normally\n",pid);
	}
	else if(pid>0 && WIFSIGNALED(status)==1)
	{
		bg_count-=1;
		fprintf(stderr,"\nProcess with pid %d exited due to a user-defined signal\n",pid);
	}	
}

void foreback(int noarg)
{
	char* comm;
	int i,ret,j,fd_input,fd_output,fd_append,pid,wpid,stdout1,stdin1,stdapp1;
	char *newproc[1000];
	for(i=0;i<1000;i++) newproc[i] = (char*)malloc(1000*sizeof(char));

	for(i=0;i<noarg;i++)
	{
		strcpy(newproc[i],eachcommand[i]);
	}

	check_redirect(noarg);
	if(input_flag!=-1)
	{
		stdin1 = dup(0);
		newproc[input_flag] = NULL;
		fd_input = open(input_file,O_RDONLY,0);
		if(fd_input<0) {
			perror("Error");
			close(stdin1);
			return;
		}
		if(dup2(fd_input,0)<0) perror("Error");
	}

	if(output_flag!=-1)
	{
		stdout1 = dup(1);
		newproc[output_flag] = NULL;
		fd_output = open(output_file, O_CREAT | O_WRONLY | O_TRUNC,0644);
		if(fd_output<0) perror("Error");
		if(dup2(fd_output,1)<0) perror("Error");
	}
	if(append_flag!=-1)
	{
		stdapp1 = dup(1);
		newproc[append_flag] = NULL;
		fd_append = open(append_file, O_CREAT | O_WRONLY | O_APPEND,0644);
		if(fd_append<0) perror("Error");
		if(dup2(fd_append,1)<0) perror("Error");
	}

	if(strcmp(eachcommand[noarg-1],"&")==0)
	{
		bg_flag = 1;
		newproc[noarg-1] = NULL;
	}
	else
	{
		newproc[noarg] = NULL;
	}
	signal(SIGCHLD,handler);
	pid = fork();
	// child_pid = pid;
	
	if(pid<0)
	{
		perror("Error");
	}
	else if(pid>0)
	{
		shell_pid = pid;
		strcpy(forename,eachcommand[0]);
		if(bg_flag == 1)
		{
			bg_count +=1;
			printf("[%d] %d\n",bg_count, pid);
			jobs_list[prcount].id = (int) pid;
			strcpy(jobs_list[prcount].name,eachcommand[0]);
			strcpy(jobs_list[prcount].status,"RUNNING");
			prcount+=1;

			// printf("[%d] %s %s[%d]\n",1,jobs_list[prcount-1].status,jobs_list[prcount-1].name,jobs_list[prcount-1].id);
			waitpid(pid,&status,WNOHANG);
		}
		else
		{
			waitpid(pid,NULL,WUNTRACED);
		}
	}
	else if(pid == 0)
	{
		setpgid(0, 0);
		ret = execvp(newproc[0],newproc);
		if(ret == -1)
		{
			fprintf(stderr,"Error executing %s command.\n",newproc[0]);
		}
	}
	if(input_flag!=-1)
	{
		dup2(stdin1,0);
		close(stdin1);
		close(fd_input);
	}
	if(output_flag!=-1)
	{
		dup2(stdout1,1);
		close(stdout1);
		close(fd_output);
	}
	if(append_flag!=-1)
	{
		dup2(stdapp1,1);
		close(stdapp1);
		close(fd_append);
	}

	for(i=0;i<1000;i++) free(newproc[i]);
}
