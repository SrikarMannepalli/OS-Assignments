#include <stdio.h>
#include <stdlib.h>

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


    mergesort(a,0,n-1);

    for(i=0;i<n;i++) printf("%d ",a[i]);
    printf("\n");
    return 0;
}

