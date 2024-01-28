#include <stdio.h>
#include <unistd.h>

int main()
{
    int a = 4;
    printf("PID: %d\n", getpid());
    printf("Virtual address: %ld\n", &a);

    while(1);
    return 0;
}