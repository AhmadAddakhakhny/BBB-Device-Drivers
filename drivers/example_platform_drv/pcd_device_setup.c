#include<linux/module.h>
#include<linux/platform_device.h>
#include "platform_data.h"

/* Prototypes */
void pcdev_release (struct device *dev);

/* 1. Create 2 platform private data */
struct pcdev_platform_data pcdev_pdata[PLATFORM_DEVICES] = {
    [PCDEV_1] = {
        .size = DEV_BUFFER_PCDEV1,
        .serial_number = "PCDEV1XXXABC123",
        .perm = RDWR
    }, 
    [PCDEV_2] = {
        .size = DEV_BUFFER_PCDEV2,
        .serial_number = "PCDEV2XXXABC123",
        .perm = RDWR
    }
};

/* 2. create 2 platform devices */
struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0,
    .dev = {
        .platform_data = &pcdev_pdata[PCDEV_1],
        .release = pcdev_release
    }
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1,
    .dev = {
        .platform_data = &pcdev_pdata[PCDEV_2],
        .release = pcdev_release
    }
};

/* Define device release method/s */
void pcdev_release (struct device *dev) {
    pr_info("Device released\n");
}

static int __init pcdev_platform_init(void) {
    /* Register platform devices */
    pr_info("Device setup module inserted \n");
    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);

    return 0;
}

static void __exit pcdev_platform_exit(void) {
    /* Un-register platform device */
    pr_info("Device setup module removed \n");
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module which registers platform devices");
