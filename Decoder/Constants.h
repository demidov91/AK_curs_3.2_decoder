#define BLOCK_COUNT 256
#define MAX_FILE_COUNT 256
#define MAX_BLOCK_SIZE 256
#define BYTE unsigned char
///------------------------------------------
#define BYTES_FOR_FSOBJECT 5
#define BYTES_FOR_FSOBJECTNAME_LENGTH 1
///Format of data: |is it file or directory|length of file or number of files in directory - long int|BYTES_FOR_FSOBJECTNAME_LENGTH (length of next block)|name of file or directory|data...|
///                -------------------------BYTES_FOR_FSOBJECT---------------------------------------
///-------------------------------------------
