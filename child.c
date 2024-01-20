#include<stdio.h>
int main()
{
    int rc;
    printf("Parent : %d\n",getpid());
    rc=fork();
    
    if(rc==0)
    {
        printf("%d\n",getpid());
        sleep(10);
    }
    else{
        rc=fork();
        if(rc==0)
        {
            printf("%d\n",getpid());
            sleep(10);
        }
        else
        {
            rc=fork();
            if(rc==0)
            {
                printf("%d\n",getpid());
                sleep(10);
            }
        }

        wait(NULL);
    }
    return 0;
}