#include "headers.h"

//error handling to be done
void check_redirect(int no_arg)
{
	input_flag = -1;
	output_flag = -1;
	int i;
	for(i=0;i<no_arg;i++)
	{
		if(strcmp(eachcommand[i],"<")==0)
		{
			input_flag = i;
			strcpy(input_file,eachcommand[i+1]);
			// eachcommand[i] = NULL;
		}
		else if(strcmp(eachcommand[i],">")==0)
		{
			output_flag = i;
			strcpy(output_file,eachcommand[i+1]);
			// eachcommand[i] = NULL;
		}
		else if(strcmp(eachcommand[i],">>")==0)
		{
			append_flag = i;
			strcpy(append_file,eachcommand[i+1]);
			// eachcommand[i] = NULL;
		}
	}
}

int check_pipe(int no_arg)
{
	int i;
	for(i=0;i<no_arg;i++)
	{
		if(strcmp(eachcommand[i],"|")==0)
		{
			return 1;
		}
	}
	return 0;
}

void do_pipe(char** command,int command_no)
{
	int i,noeacharg,j,ini_fd=0,noarg;
	pid_t newprc;
	int fildes[2];
	noarg = getArgs2(command_no);
	int stdin1 = dup(0);
	int stdout1 = dup(1);
	int fdin = dup(0);
	int fdout,ret;
	int inp_flag;
	int out_flag;
	char* inp_file = (char*)malloc(100*sizeof(char));
	char* out_file = (char*)malloc(100*sizeof(char));

	for(i=0;i<noarg;i++)
	{
		if(dup2(fdin,0)<0) perror("Error");
		close(fdin);
		if(i==noarg-1) fdout = dup(stdout1);
		else
		{	
			pipe(fildes);
			fdout = fildes[1];
			fdin=fildes[0];
		}
		if(dup2(fdout,1)<0) perror("Error");
		close(fdout);	
		noeacharg = getArgs3(i);
		foreback(noeacharg);
	}
	if(dup2(stdin1,0)<0) perror("Error");
	if(dup2(stdout1,1)<0) perror("Error");
	close(stdin1);
	close(stdout1);
}
