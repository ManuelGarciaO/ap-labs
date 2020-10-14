#include <stdio.h>
#include "logger.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 128

char *base64_encode(char *plain);
char *base64_decode(char *cipher);
static void sigHandler(int sig);
static void encode(char *input);
static void decode(char *input);
static char *getNameEncoded(char *originalName);
static char *getNameDecoded(char *originalName);

char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int fileSize;
int bytesProcessed;
int main(int argc, char **argv)
{
    if (argc != 3)
    {
        errorf("Invalid number of arguments");
        exit(0);
    }
    fileSize = 1;
    bytesProcessed = 0;
    if (signal(SIGINT, sigHandler) == SIG_ERR || signal(SIGUSR1, sigHandler) == SIG_ERR)
    {
        errorf("Error with signal");
        exit(0);
    }

    if (!strcmp(argv[1], "--encode"))
    {
        encode(argv[2]);
    }
    else if (strcmp(argv[1], "--decode") == 0)
    {
        decode(argv[2]);
    }
    else
    {
        errorf("Error. Invalid arguments\n");
    }

    return 0;
}

static void encode(char *input)
{
    int inputFd, outputFd, openFlags, filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE + 1];
    char *encodedString;
    inputFd = open(input, O_RDONLY);
    if (inputFd == -1)
    {
        errorf("Error opening file %s\n", input);
        exit(0);
    }
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    char *output = getNameEncoded(input);
    outputFd = open(output, openFlags, filePerms);
    if (outputFd == -1)
    {
        errorf("Error opening file %s\n", output);
        exit(0);
    }

    fileSize = lseek(inputFd, 0, SEEK_END);
    if (fileSize == -1)
    {
        errorf("Error getting file size\n");
        exit(0);
    }
    if (lseek(inputFd, -fileSize, SEEK_END) == -1)
    {
        errorf("Error returning to the beginning of the file.\n");
        exit(0);
    }

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        buf[numRead] = '\0';
        encodedString = base64_encode(buf);

        if (write(outputFd, encodedString, strlen(encodedString)) < 0)
        {
            errorf("Error writing the file %s\n",output);
        }
        bytesProcessed += numRead;
    }

    if (close(outputFd) == -1)
        errorf("Error closing the fle output\n");
    if (close(inputFd) == -1)
        errorf("Error closing the fle input\n");
}

static void decode(char *input)
{
    int inputFd,outputFd, openFlags, filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE + 1];
    char *encodedString;
    inputFd = open(input, O_RDONLY);
    if (inputFd == -1)
    {
        errorf("Error opening file %s\n", input);
        exit(0);
    }
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    char *output = getNameDecoded(input);
    outputFd = open(output, openFlags, filePerms);
    if (outputFd == -1)
    {
        errorf("Error opening file %s\n", output);
        exit(0);
    }

    fileSize = lseek(inputFd, 0, SEEK_END);
    if (fileSize == -1)
    {
        errorf("Error getting file size\n");
        exit(0);
    }
    if (lseek(inputFd, -fileSize, SEEK_END) == -1)
    {
        errorf("Error returning to the beginning of the file.\n");
        exit(0);
    }

    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        buf[numRead] = '\0';
        encodedString = base64_decode(buf);

        if (write(outputFd, encodedString, strlen(encodedString)) < 0)
        {
            errorf("Error writing the file %s\n",output);
        }
        bytesProcessed += numRead;
    }

    if (close(outputFd) == -1)
        errorf("Error closing the fle output\n");
    if (close(inputFd) == -1)
        errorf("Error closing the fle input\n");
}

static void sigHandler(int sig)
{
    infof("Current progress: %.2f%%\n", (bytesProcessed * 100.0) / fileSize);
}

static char *getNameEncoded(char *originalName)
{
    int originalSize = strlen(originalName) - 4;
    char *name = malloc(originalSize + 1);
    for (size_t i = 0; i < originalSize; i++)
    {
        name[i] = originalName[i];
    }
    name[originalSize] = '\0';
    char *add = "-encoded";
    int newSize = strlen(originalName) + strlen(add);
    char *newName = malloc(newSize + 1);
    strcat(newName, "");
    strcat(newName, name);
    strcat(newName, add);
    strcat(newName, ".txt");
    return newName;
}
static char *getNameDecoded(char *originalName)
{
    int originalSize = strlen(originalName) - 12;
    char *name = malloc(originalSize + 1);
    for (size_t i = 0; i < originalSize; i++)
    {
        name[i] = originalName[i];
    }
    name[originalSize] = '\0';
    char *add = "-decoded";
    int newSize = strlen(originalName) + strlen(add);
    char *newName = malloc(newSize + 1);
    strcat(newName, "");
    strcat(newName, name);
    strcat(newName, add);
    strcat(newName, ".txt");
    return newName;
}

//credits for Phidias Chiang 
char *base64_encode(char *plain)
{

    char counts = 0;
    char buffer[3];
    char *cipher = malloc(strlen(plain) * 4 / 3 + 4);
    int i = 0, c = 0;

    for (i = 0; plain[i] != '\0'; i++)
    {
        buffer[counts++] = plain[i];
        if (counts == 3)
        {
            cipher[c++] = base46_map[buffer[0] >> 2];
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
            cipher[c++] = base46_map[buffer[2] & 0x3f];
            counts = 0;
        }
    }

    if (counts > 0)
    {
        cipher[c++] = base46_map[buffer[0] >> 2];
        if (counts == 1)
        {
            cipher[c++] = base46_map[(buffer[0] & 0x03) << 4];
            cipher[c++] = '=';
        }
        else
        { // if counts == 2
            cipher[c++] = base46_map[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
            cipher[c++] = base46_map[(buffer[1] & 0x0f) << 2];
        }
        cipher[c++] = '=';
    }

    cipher[c] = '\0'; /* string padding character */
    return cipher;
}

char *base64_decode(char *cipher)
{

    char counts = 0;
    char buffer[4];
    char *plain = malloc(strlen(cipher) * 3 / 4);
    int i = 0, p = 0;

    for (i = 0; cipher[i] != '\0'; i++)
    {
        char k;
        for (k = 0; k < 64 && base46_map[k] != cipher[i]; k++)
            ;
        buffer[counts++] = k;
        if (counts == 4)
        {
            plain[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
            if (buffer[2] != 64)
                plain[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
            if (buffer[3] != 64)
                plain[p++] = (buffer[2] << 6) + buffer[3];
            counts = 0;
        }
    }

    plain[p] = '\0'; /* string padding character */
    return plain;
}