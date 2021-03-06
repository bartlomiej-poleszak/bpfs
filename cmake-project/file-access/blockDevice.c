#include "blockDevice.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

off_t seekToBlock(BlockDevice *partition, int blockNumber)
{
    off_t desiredOffset = blockNumber * partition->blockSize;
    off_t offset = lseek(partition->descriptor, desiredOffset, SEEK_SET);

    if (offset < 0) {
        perror("seekToBlock");
        exit(1);
    }
    if (desiredOffset != offset) {
        printf("Desired offset is not reached!");
    }
    return offset;
}

///
/// \brief openBlockDevice
/// \param path
/// \return file descriptor if file is opened, terminates if error occured
///

BlockDevice openBlockDevice(const char *path, int blockSize)
{
    BlockDevice result;
    result.descriptor = open(path, O_RDWR | O_SYNC);
    result.blockSize = blockSize;
    if (result.descriptor < 0) {
        perror("openBlockDevice");
        exit(1);
    }

    return result;
}

void closeBlockDevice(BlockDevice *partition)
{
    close(partition->descriptor);
}

void readBlockDevice(BlockDevice *partition, int blockNumber, char *buffer)
{
    seekToBlock(partition, blockNumber);
    int bytesRead = read(partition->descriptor, buffer, partition->blockSize);
    if (bytesRead != partition->blockSize) {
        perror("readBlock");
        exit(1);
    }
}


void writeBlockDevice(BlockDevice *partition, int blockNumber, const char *buffer)
{
    seekToBlock(partition, blockNumber);
    int bytesWritten = write(partition->descriptor, buffer, partition->blockSize);
    if (bytesWritten  != partition->blockSize) {
        perror("writeBlock");
        exit(1);
    }
}


unsigned long long blockCount(BlockDevice *partition)
{
    char buffer[partition->blockSize];
    seekToBlock(partition, 0);

    unsigned long long result = 0;
    if ((result = lseek64(partition->descriptor, 0, SEEK_END)) < 0) {
        perror("blockCount");
        exit(1);
    }
    result /= partition->blockSize;
    printf("blockCount: %lld\n", result);
    return result;
}
