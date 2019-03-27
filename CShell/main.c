#include "headers.h"

void ctrlc(int sig)
{
	if(shell_pid!=-1)
	{	
		int ret= kill(shell_pid,SIGINT);
		// if(ret==-1) perror("Error");
	}
}


int main(int argc,char* argv[])
{
	int ret,t,no_arg,i,j,no_commands,ls_present,pinfo_present,clock_present =0,remind_present = 0,pipe_present = 0;
	char* command[1000];
	extern int prcount;
	prcount = 0;
	hostname = (char*)malloc(sizeof(char));
	home = (char*)malloc(100*sizeof(char));
	displayCWD = (char*)malloc(100*sizeof(char));
	cwd = (char*)malloc(100*sizeof(char));
	prevdir = (char*)malloc(100*sizeof(char));
	input_file = (char*)malloc(100*sizeof(char));
	output_file = (char*)malloc(100*sizeof(char));
	append_file = (char*)malloc(100*sizeof(char));
	signal(SIGINT,ctrlc);
	signal(SIGTSTP,ctrlz);
	getcwd(cwd,100);
	strcpy(home,cwd);
	strcpy(displayCWD,cwd);
	strcpy(prevdir,home);
	forename = (char*)malloc(100*sizeof(char));
	bg_count = 0;

	while(1)
	{
		for(i=0;i<1000;i++) command[i] = (char*)malloc(1000*sizeof(char));
		for(i=0;i<1000;i++) arrc[i] = (char*)malloc(1000*sizeof(char));
		for(i=0;i<1000;i++) eachcommand[i] = (char*)malloc(1000*sizeof(char));
		for(i=0;i<1000;i++) pipedcommand[i] = (char*)malloc(1000*sizeof(char));
		for(i=0;i<1000;i++) eachpipe[i] = (char*)malloc(1000*sizeof(char));
		print_prompt();
		no_commands = nocommands(command);
		for(i=0;i<no_commands;i++) strcpy(arrc[i],command[i]);

		for(i=1;i<=no_commands;i++)
		{
			bg_flag = 0;
			input_flag = -1;
			output_flag = -1;
			append_flag = -1;
			no_arg = getArgs1(command,i);
			int chus = check_user(no_arg);
			pipe_present = check_pipe(no_arg);
			if(pipe_present==1 && chus == 0)
			{
				do_pipe(command,i);
			}
			else if(chus==0)
			{
				if(check_builtin()==1)
				{
					//builtin
					builtin(no_arg);
				}
				else
				{
					ls_present = check_ls(no_arg);
					if(check_clock(no_arg)==1) clock1(eachcommand[2],eachcommand[4]);
					else if(check_remindme(no_arg)==1) remindme(eachcommand[1],eachcommand[2],no_arg);
					else if(check_pinfo(no_arg)==1) dopinfo(no_arg);
					else if(ls_present == 0) foreback(no_arg);
				}
			}
		}
		for(i=0;i<1000;i++) 
		{
			free(command[i]);
			free(eachcommand[i]);
			free(pipedcommand[i]);
			free(arrc[i]);
		}
	}

	free(hostname);
	free(home);
	free(displayCWD);
	free(cwd);
	free(prevdir);
	free(forename);
	return 0;
}
