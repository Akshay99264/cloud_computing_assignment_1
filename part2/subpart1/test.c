#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
typedef struct
{
    int pid;
    unsigned long virtual_address;
    unsigned long physical_address;
} get_phys;

typedef struct
{
    unsigned long physical_address;
    int value;
} chang_para;

#define QUERY_GET_PHYSICAL_ADDR _IOR('q', 1, unsigned long)
#define QUERY_CHANGE_VALUE _IOW('q', 2, chang_para)
int main()
{
    int n = 6;
    unsigned long virtual_address = 0, physical_address = 0;
    virtual_address = &n;
    get_phys temp;
    temp.pid = getpid();
    temp.virtual_address = virtual_address;

    int fd = open("/dev/v2p_p2v_device", O_RDWR);
    if (fd == -1) {
        printf("Error: opening device file\n");
        return 0;
    }
    if (ioctl(fd, QUERY_GET_PHYSICAL_ADDR, &temp) == -1) {
        printf("Error: ioctl call\n");
        close(fd);
        return 0;
    }
    printf("PID: %d, virtual address: %ld, physical address: %ld\n", temp.pid, temp.virtual_address, temp.physical_address);
    
    chang_para change_value;
    change_value.physical_address = temp.physical_address;
    change_value.value = 255;
    printf("Previous value: %d\n",n);
    if (ioctl(fd, QUERY_CHANGE_VALUE, &change_value) == -1) {
        printf("Error while making ioctl call\n");
        close(fd);
    }
    printf("Updated value: %d\n", n);
    return 0;
}