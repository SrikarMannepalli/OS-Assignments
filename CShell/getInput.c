#include "headers.h"

int nocommands(char** command)
{
	inp = (char*)malloc(1000*sizeof(char));
	char* tok;
	size_t buff_size = 1000;
	char delim[4];
	int i,n;
	delim[0] = ';';
	getline(&inp,&buff_size,stdin);
	strcpy(arr,inp);
	inp[strlen(inp)-1] = ';';
	tok = &inp[0];
	n=0,i=0;
	while((tok = strtok(tok,delim)) !=NULL)
	{
		token[i]  = tok;
		strcpy(command[i],tok);
		tok = NULL;
		n++;
		i++;
	}
	free(tok);
	return n;
}

int getArgs1(char** command,int command_no)
{
	char* tok;
	int argno = 0;
	int i;
	char* delim=(char*)malloc(4*sizeof(char));
	strcpy(delim," ");
	strcat(delim,"\t");
	tok = command[command_no-1];
	i=0;

	while((tok = strtok(tok,delim))!= NULL)
	{
		strcpy(eachcommand[i],tok);
		tok = NULL;
		i++;
		argno++;
		eachcommand[i][strlen(eachcommand[i])] = ' ';
	}

	free(tok);
	return argno;
}

int getArgs2(int command_no)
{
	char* tok;
	int argno = 0;
	int i;
	char* delim=(char*)malloc(4*sizeof(char));
	strcpy(delim,"|");
	tok = arrc[command_no-1];

	i=0;

	while((tok = strtok(tok,delim))!= NULL)
	{
		strcpy(pipedcommand[i],tok);
		tok = NULL;
		i++;
		argno++;
		pipedcommand[i][strlen(pipedcommand[i])] = ' ';
	}

	free(tok);
	return argno;
}

int getArgs3(int command_no)
{
	char* tok;
	int argno = 0;
	int i;
	char* delim=(char*)malloc(4*sizeof(char));
	strcpy(delim," ");
	strcat(delim,"\t");
	tok =pipedcommand[command_no];
	i=0;
	while((tok = strtok(tok,delim))!= NULL)
	{
		strcpy(eachcommand[i],tok);
		tok = NULL;
		i++;
		argno++;
		eachcommand[i][strlen(eachcommand[i])] = ' ';
	}

	free(tok);
	return argno;
}
