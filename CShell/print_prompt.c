#include "headers.h"

void print_prompt()
{
	int ret;
	char* retu = (char*)malloc(100*sizeof(char));
	ret = gethostname(hostname,50);
	if(ret==-1)
	{
		perror("Error");
	}
	username = getenv("USER");
	retu = getcwd(cwd,100);
	if(retu==NULL)
	{
		perror("Error");
		return;
	}
	convertCWD(cwd);
	printf("<%s@%s:%s>",username,hostname,displayCWD);
	free(retu);
}
