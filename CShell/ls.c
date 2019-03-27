#include "headers.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>

int check_ls(int noarg)
{
	int i;
	for(i=0;i<noarg;i++)
	{
		if(strcmp(eachcommand[i],">") == 0)
			return 0;
		else if(strcmp(eachcommand[i],"<") == 0)
			return 0;
		else if(strcmp(eachcommand[i],">>") == 0)
			return 0;
	}
	int ls_l =0,ls_a =0,ls_al =0,other =0;
	if(strcmp(eachcommand[0],"ls") == 0)
	{
		 if(strcmp(eachcommand[noarg-1],"&")==0)
		 {
		     return 0;
		 }
		for(i=1;i<noarg;i++)
		{
			if(strcmp(eachcommand[i],"-l") ==0) ls_l =1;
			else if(strcmp(eachcommand[i],"-a") ==0) ls_a =1;
			else if(strcmp(eachcommand[i],"-la") ==0 || strcmp(eachcommand[i],"-al")==0)
			{
				ls_al = 1;
			}
			else
			{
				if(eachcommand[i][0] == '-') other = 1;
			}
		}
		if(other == 1)
		{
			printf("no such flag.\n");
			return 1;
		}
		if ((ls_a==1 && ls_l ==1))
		{
			dols_l(noarg,ls_a);
		}
		else if(ls_al == 1)
		{
			dols_l(noarg,ls_al);
		}
		else if(ls_l == 1)
		{
			dols_l(noarg,ls_a);
		}
		else if(ls_a == 1)
		{
			do_ls(noarg,ls_a);
		}
		else
		{
			do_ls(noarg,ls_a);
		}
		return 1;
	}
	return 0;
}

void dols_l(int noarg,int aflag)
{
	DIR* dir;
	int ret,file_count=0,i;
	char datetime[100];
	char* curr=(char*)malloc(sizeof(char));
	struct dirent* file;
	struct stat st;
	struct group* group;
	struct passwd* pw;
	char* buf= (char*)malloc(100*sizeof(char));
	char* fname = (char*)malloc(100*sizeof(char));
	for(i=1;i<noarg;i++)
	{
		if(eachcommand[i][0] != '-') file_count++;
	}
	if(file_count==0)
	{
		strcpy(curr,".");
		dir = opendir(curr);
		printf("%s:\n",curr);
		while((file=readdir(dir))!=NULL)
		{
			stat(file->d_name,&st);
			if(aflag==1)
			{
				print_all(st);
				group = getgrgid(st.st_gid);
				pw = getpwuid(st.st_uid);
				printf(" %s", pw->pw_name);
				printf(" %s", group->gr_name);
				strftime(datetime, 100, "%h %d %H:%M", localtime(&(st.st_ctime)));
				printf(" %s    ", datetime);
				printf(" %4ld", st.st_size);
				printf(" \t%s\n", file->d_name);
			}
			else
			{
				if(file->d_name[0]!= '.')
				{
					print_all(st);
					group = getgrgid(st.st_gid);
					pw = getpwuid(st.st_uid);
					printf(" %s", pw->pw_name);
					printf(" %s", group->gr_name);
					strftime(datetime, 100, "%h %d %H:%M", localtime(&(st.st_ctime)));
					printf(" %s    ", datetime);
					printf(" %4ld", st.st_size);
					printf(" \t%s\n", file->d_name);
				}
			}
		}
		closedir(dir);
		return;
	}

	for(i=1;i<noarg;i++)
	{
		if(eachcommand[i][0]!= '-')
		{
			if(eachcommand[i][0] == '~')
			{
				strcpy(fname,home);
				strcat(fname,&eachcommand[i][1]);
			}
			else
			{
				strcpy(fname,eachcommand[i]);
			}
			dir = opendir(fname);
			if(dir == NULL)
			{
				ret = stat(fname,&st);
				if(ret!=0)
				{
					printf("File does not exist.\n");
					return;
				}
				else
				{
					printf("%s\n",fname);
					return;
				}
			}
			printf("%s:\n",fname);
			while((file=readdir(dir))!=NULL)
			{
				strcpy(buf,eachcommand[i]);
				strcat(buf,"/");
				strcat(buf,file->d_name);
				stat(buf,&st);
				if(aflag==1)
				{
					print_all(st);
					group = getgrgid(st.st_gid);
					pw = getpwuid(st.st_uid);
					printf(" %s", pw->pw_name);
					printf(" %s", group->gr_name);
					strftime(datetime, 100, "%h %d %H:%M", localtime(&(st.st_ctime)));
					printf(" %s    ", datetime);
					printf(" %4ld", st.st_size);
					printf(" \t%s\n", file->d_name);
				}
				else
				{
					if(file->d_name[0]!='.')
					{
						print_all(st);
						group = getgrgid(st.st_gid);
						pw = getpwuid(st.st_uid);
						printf(" %s", pw->pw_name);
						printf(" %s", group->gr_name);
						strftime(datetime, 100, "%h %d %H:%M", localtime(&(st.st_ctime)));
						printf(" %s    ", datetime);
						printf(" %4ld", st.st_size);
						printf(" \t%s\n", file->d_name);
					}
				}
			}
			closedir(dir);
		}
	}
	free(curr);
	free(buf);
	free(fname);
}

void do_ls(int noarg,int aflag)
{
	DIR* dir;
	int ret,file_count=0,i;
	char* curr=(char*)malloc(sizeof(char));
	struct dirent* file;
	struct stat st;
	char* fname = (char*)malloc(100*sizeof(char));
	for(i=1;i<noarg;i++)
	{
		if(eachcommand[i][0] != '-') file_count++;
	}
	if(file_count==0)
	{
		strcpy(curr,".");
		dir = opendir(curr);
		printf("%s:\n",curr);
		while((file=readdir(dir))!=NULL)
		{
			if(aflag==1) printf("%s\n",file->d_name);
			else
			{
				if(file->d_name[0]!='.') printf("%s\n",file->d_name);
			}
		}
		closedir(dir);
		return;
	}
	for(i=1;i<noarg;i++)
	{
		if(eachcommand[i][0]!= '-')
		{
			if(eachcommand[i][0] == '~')
			{
				strcpy(fname,home);
				strcat(fname,&eachcommand[i][1]);
			}
			else
			{
				strcpy(fname,eachcommand[i]);
			}

			dir = opendir(fname);
			if(dir == NULL)
			{
				ret = stat(fname,&st);
				if(ret!=0)
				{
					printf("File does not exist.\n");
					return;
				}
				else
				{
					printf("%s\n",fname);
					return;
				}
			}
			printf("%s:\n",fname);
			while((file=readdir(dir))!=NULL)
			{
				if(aflag==1) printf("%s\n",file->d_name);
				else
				{
					if(file->d_name[0]!='.') printf("%s\n",file->d_name);
				}
			}
			closedir(dir);
		}
	}
	free(curr);
	free(fname);
}

void print_all(struct stat st)
{
	char datetime[100];
	printf( (S_ISDIR(st.st_mode)) ? "d" : "-");
	printf( (st.st_mode & S_IRUSR) ? "r" : "-");
	printf( (st.st_mode & S_IWUSR) ? "w" : "-");
	printf( (st.st_mode & S_IXUSR) ? "x" : "-");
	printf( (st.st_mode & S_IRGRP) ? "r" : "-");
	printf( (st.st_mode & S_IWGRP) ? "w" : "-");
	printf( (st.st_mode & S_IXGRP) ? "x" : "-");
	printf( (st.st_mode & S_IROTH) ? "r" : "-");
	printf( (st.st_mode & S_IWOTH) ? "w" : "-");
	printf( (st.st_mode & S_IXOTH) ? "x" : "-");
	printf(" %4ld ", st.st_nlink);
}
