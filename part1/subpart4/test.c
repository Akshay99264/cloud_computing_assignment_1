#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *ptr;
    printf("Process ID: %d\n", getpid());
    size_t stage_size = 1024 * 4096;

    for (int stage = 1; stage <= 5; ++stage) {
        printf("Stage %d - Allocating %lu KB\n", stage, stage_size / 1024);
        ptr = (int *)malloc(stage_size);

        stage_size *= 2;
        sleep(20); 
        free(ptr);
    }

}
