#include <stdio.h>

int main()
{
    int a = 4;
    printf("PID : %d\n", getpid());
    printf("%ld\n", &a);

    while(1);
}