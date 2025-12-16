#define DEV_BUFFER_PCDEV1    512
#define DEV_BUFFER_PCDEV2    1024

/* MACROS */
#define MAX_DEVICES          10
#define NO_OF_DEVICES        4
#define DEV_BUFFER_PCDEV1    512
#define DEV_BUFFER_PCDEV2    1024
#define DEV_BUFFER_PCDEV3    1024
#define DEV_BUFFER_PCDEV4    512
#define PCDEV_MINOR_BASE_NUMBER    0

typedef enum {
    PCDEV_1 = 0,
    PCDEV_2 = 1,
    PCDEV_3 = 2,
    PCDEV_4 = 3,
    PCDEV_INVALID = 4
} ePCDEV;

enum ePERMISSION {
    RDONLY = 0,
    WRONLY = 1,
    RDWR = 2,
    INVALID_PERM = -EPERM
};


struct pcdev_platform_data {
    int size;
    int perm;
    const char *serial_number;
};
