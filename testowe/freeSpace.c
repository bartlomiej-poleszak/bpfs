#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

// blockSize in bytes
// return: number of free blocks
// blockSize*return = free space
int freeBlocks(int deviceDescriptor, int blockSize) {
    char buffer[1024];
    int result = 0;
    lseek(deviceDescriptor, 0, SEEK_SET);
    int bytesRead;
    while ( (bytesRead = read(deviceDescriptor, buffer, blockSize)) == blockSize) {
        result++;
    }
    return result;
}

int main() {
    int partition = open("/dev/sdb1", O_RDONLY);
    printf("free blocks: %d", freeBlocks(partition, 1024));
    close(partition);
}
