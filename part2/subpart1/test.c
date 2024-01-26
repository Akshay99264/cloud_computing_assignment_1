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
    char value;
} chang_para;

#define QUERY_GET_PHYSICAL_ADDR _IOR('q', 1, unsigned long)
#define QUERY_CHANGE_VALUE _IOW('q', 2, chang_para)
int main()
{
    size_t size = 1;  // 1 byte
    unsigned char *byteMemory = (unsigned char *)malloc(size);

    if (byteMemory == NULL) {
        // Allocation failed, handle the error
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    *byteMemory='6';
    unsigned long virtual_address = 0, physical_address = 0;
    virtual_address = byteMemory;
    get_phys temp;
    temp.pid = getpid();
    temp.virtual_address = virtual_address;
    printf("Virtual address: %ld, Value: %c\n\n", byteMemory, *byteMemory);
    
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
    printf("******* Mapping of virtual to physical address *******\n");
    printf("PID: %d, virtual address: %ld, physical address: %ld\n\n", temp.pid, temp.virtual_address, temp.physical_address);
    
    chang_para change_value;
    change_value.physical_address = temp.physical_address;
    change_value.value = '5';
    printf("******* Updating the value *******\n");
    printf("Previous value: %c\n",*byteMemory);
    if (ioctl(fd, QUERY_CHANGE_VALUE, &change_value) == -1) {
        printf("Error while making ioctl call\n");
        close(fd);
    }
    printf("Updated value: %c\n", *byteMemory);
    return 0;
}