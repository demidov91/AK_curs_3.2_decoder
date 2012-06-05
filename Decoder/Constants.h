#define BLOCK_COUNT 256
#define MAX_FILE_COUNT 256
#define MAX_THREAD_COUNT MAX_FILE_COUNT
#define MAX_BLOCK_SIZE 256
#define BYTE unsigned char
///------------------------------------------
#define BYTES_FOR_FSOBJECT 5
#define BYTES_FOR_FSOBJECTNAME_LENGTH 1
///Format of data: |is it file or directory|length of file or number of files in directory - long int|BYTES_FOR_FSOBJECTNAME_LENGTH (length of next block)|name of file or directory|data...|
///                -------------------------BYTES_FOR_FSOBJECT---------------------------------------
///-------------------------------------------
#define INFORMATION_PART_LENGTH MAX_THREAD_COUNT*2+BLOCK_COUNT
#define DIRECTORY_BYTE 1
#define FILE_BYTE 2

#define INVALID_FILE_FORMAT 1
#define INVALID_PASSWORD 2
#define INVALID_KEY 3