#include<stdio.h>
int main()
{
    printf("ID %d\n",getpid());
    if(fork() ==0)
    {
        printf("ID %d\n",getpid());
    }
     
    if(fork() ==0)
    {
        printf("ID %d\n",getpid());
    }
   
    if(fork() ==0)
    {
        printf("ID %d\n",getpid());
    }
    
    if(fork() ==0)
    {
        printf("ID %d\n",getpid());
    }
    wait(NULL);
    while(1);
    return 0;

}