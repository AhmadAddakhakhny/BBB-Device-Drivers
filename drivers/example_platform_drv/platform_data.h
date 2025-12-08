#define PLATFORM_DEVICES     2
#define DEV_BUFFER_PCDEV1    512
#define DEV_BUFFER_PCDEV2    1024

typedef enum {
    PCDEV_1 = 0,
    PCDEV_2 = 1,
    PCDEV_INVALID = 2
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
