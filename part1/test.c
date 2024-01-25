#include<stdio.h>
int main()
{
    int a=5;
    printf("%ld\n",&a);
    printf("PID: %d\n",getpid());
    while(1);
    return 0;
}