#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#define QUERY_GET_CONSTANT _IOR('q', 1, int)
 
void get_constant(int fd)
{
    int q;
 
    if (ioctl(fd, QUERY_GET_CONSTANT, &q) == -1)
    {
        perror("getConstant ioctl get");
    }
    else
    {
        printf("Returned value of constant: %d\n", q);
    }
} 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/getConstant";
    int fd;
    fd = open(file_name, O_RDONLY);
    if (fd == -1)
    {
        perror("getConstant open");
        return 2;
    }
    get_constant(fd);
    close (fd);
 
    return 0;
}