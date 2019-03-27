#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct args 
{
    int *arr;
    int l;
    int r;
} args;

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

void *msort(void *given_args)
{
    int i,j,l,r,temp;
    args* args_given=(args*)malloc(sizeof(args));
    args_given=(args*)given_args;
    l=args_given->l;
    r=args_given->r;
    int *a=args_given->arr;
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
        args* argu;
        argu=(args*)malloc(sizeof(args));
        pthread_t tid1,tid2;
        argu->arr=a;
        argu->l=l;
        argu->r=m;
        // printf("before left call\n");
        // printf("%ld\n",pthread_self());

        pthread_create(&tid1,NULL,msort,(void*)argu);
        // printf("left : %ld\n",tid1);
        pthread_join(tid1,NULL);
        
        argu->arr=a;
        argu->l=m+1;
        argu->r=r;
        // printf("before right call\n");
        // printf("%ld\n",pthread_self());

        pthread_create(&tid2,NULL,msort,(void*)argu);
        // printf("right : %ld\n",tid2);
        pthread_join(tid2,NULL);

        // printf("before merge call\n");
        // printf("%ld\n",pthread_self());

        merge(a,l,r);
    }
}

int main(int argc,char* argv[]) 
{
    int i,n,b[1000005],s_id;
    int *a;
    clock_t t1,t2;
    n=10000;
    // scanf("%d",&n);
    a=(int*)malloc(n*sizeof(int));

    for(i=0;i<n;i++) 
    {
        // a[i] = n-i;
        scanf("%d",&a[i]);
        // b[i]=a[i];
    }
    
    // printf("with threads\n");
    args* argu;
    pthread_t tid;
    argu=(args*)malloc(sizeof(args));
    argu->arr=a;
    argu->l=0;
    argu->r=n-1;
    // t1=clock();
    pthread_create(&tid,NULL,msort,(void*)argu);
    pthread_join(tid,NULL);
    // t2=clock();
    // printf("%f\n",(t2-t1)/(double)CLOCKS_PER_SEC);

    
    for(i=0;i<n;i++) printf("%d ",a[i]);
    printf("\n");


    // printf("without threads\n");
    // for(i=0;i<n;i++) a[i]=b[i];
    // t1=clock();
    // mergesort(a,0,n-1);
    // t2=clock();
    // printf("%f\n",(t2-t1)/(double)CLOCKS_PER_SEC);
    // for(i=0;i<n;i++) printf("%d ",a[i]);
    // printf("\n");
    return 0;
}

