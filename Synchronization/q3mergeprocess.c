#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void merge(int a[],int l,int r)
{
    int b[1000005];
    int m=(l+r)/2;
    int i=l,j=m+1,k=l;
    while(i<=m && j<=r)
    {
        if(a[i]<a[j]) 
        {
            b[k] = a[i];
            k++;
            i++;
        }
        else 
        {
            b[k]=a[j];
            k++;
            j++;
        }
    }
    while(i<=m)
    {
        b[k] = a[i];
        k++;
        i++;
    }
    while(j<=r)
    {
        b[k]=a[j];
        k++;
        j++;
    }
    for(i=l;i<=r;i++) a[i]=b[i];

}

void mergesort(int a[],int l, int r)
{
    int i,j,temp;
    if(r-l<=4) 
    {
        for(i=l;i<=r;i++)
        {
            for(j=i+1;j<=r;j++)
            {
                if(a[i]>a[j])
                {
                    temp=a[i];
                    a[i]=a[j];
                    a[j]=temp;
                }
            }
        }
    }
    else
    {   
        int m=(l+r)/2;
        mergesort(a,l,m);
        mergesort(a,m+1,r);
        merge(a,l,r);
    }
}

void msort(int a[],int l,int r)
{
    int i,j,temp;

    if(r-l<=4) 
    {
        for(i=l;i<=r;i++)
        {
            for(j=i+1;j<=r;j++)
            {
                if(a[i]>a[j])
                {
                    temp=a[i];
                    a[i]=a[j];
                    a[j]=temp;
                }
            }
        }
    }
    else
    {   
        int m=(l+r)/2;
        pid_t leftproc,rightproc;
        int ret1,ret2;
        leftproc=fork();
        if(leftproc<0) 
        {
            perror("Error");
        }
        else if(leftproc==0)
        {
            msort(a,l,m);
            exit(0);
        }
        else 
        {
            rightproc=fork();
            if(rightproc<0)
            {
                perror("Error");
            }
            else if(rightproc==0)
            {
                msort(a,m+1,r);
                exit(0);
            }
        }
        waitpid(leftproc,&ret1,0);
        waitpid(rightproc,&ret2,0);
        merge(a,l,r);
    }
}

int main(int argc,char* argv[]) 
{
    int i,n,b[1000005],s_id;
    int *a;
    clock_t t1,t2;
    n=10000;
    s_id=shmget(IPC_PRIVATE,n*sizeof(int),(IPC_CREAT|0600));
    if(s_id==-1)
    {
        perror("shmget");
        exit(1);
    }
    a=shmat(s_id,NULL,0);
    if(a==(int *)-1)
    {
        perror("shmat");
        exit(1);
    }
    
    for(i=0;i<n;i++) 
    {
        scanf("%d",&a[i]);
        // a[i] = n-i;
        // b[i]=a[i];
    }
  

    printf("Using processes:\n");

    // t1=clock();
    msort(a,0,n-1);
    // t2=clock();

    
    for(i=0;i<n;i++) printf("%d ",a[i]);
    printf("\n");

    // printf("%f\n",(t2-t1)/(double)CLOCKS_PER_SEC);

    // printf("normal merge sort:\n");
    // for(i=0;i<n;i++) a[i]=b[i];
    // t1=clock();
    // mergesort(a,0,n-1);
    // t2=clock();
    // printf("%f\n",(t2-t1)/(double)CLOCKS_PER_SEC);
    // for(i=0;i<n;i++) printf("%d ",a[i]);
    // printf("\n");
    return 0;
}

