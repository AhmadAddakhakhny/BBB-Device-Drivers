#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/platform_device.h>
#include "platform_data.h"


/* Device private data */
struct pcdev_private_data {
    struct pcdev_platform_data pdata;
    char *buffer;
    dev_t device_num;
    struct cdev cdev;
};

/* Driver private data */
struct pcdrv_private_data {
    int total_devices;
    dev_t device_num_base;
    struct class* class_pcd;
    struct device* pcd_device;
    // struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};
struct pcdrv_private_data pcdrv_data;

/* Pseudo device's memory */
// char device_buffer_pcdev1[DEV_BUFFER_PCDEV1];
// char device_buffer_pcdev2[DEV_BUFFER_PCDEV2];
// char device_buffer_pcdev3[DEV_BUFFER_PCDEV3];
// char device_buffer_pcdev4[DEV_BUFFER_PCDEV4];

// struct pcdrv_private_data pcdrv_data = {
//     .total_devices = NO_OF_DEVICES,
//     .pcdev_data = {
//         [PCDEV_1] = {
//             .buffer = device_buffer_pcdev1,
//             .sz = DEV_BUFFER_PCDEV1,
//             .serial_number = "PCDEV1XXXABC123",
//             .perm = RDONLY
//         },

//         [PCDEV_2] = {
//             .buffer = device_buffer_pcdev2,
//             .sz = DEV_BUFFER_PCDEV2,
//             .serial_number = "PCDEV2XXXABC123",
//             .perm = WRONLY
//         },

//         [PCDEV_3] = {
//             .buffer = device_buffer_pcdev3,
//             .sz = DEV_BUFFER_PCDEV3,
//             .serial_number = "PCDEV3XXXABC123",
//             .perm = RDWR
//         },

//         [PCDEV_4] = {
//             .buffer = device_buffer_pcdev4,
//             .sz = DEV_BUFFER_PCDEV4,
//             .serial_number = "PCDEV4XXXABC123",
//             .perm = RDWR
//         }
//     }
// };

/* Function prototypes */
int pcd_platform_driver_probe (struct platform_device* dev);
void pcd_platform_driver_remove(struct platform_device* dev);
int check_permission(enum ePERMISSION dev_perm, int access_mode);
int pcd_open (struct inode *inode, struct file *filp);
int pcd_release (struct inode *inode, struct file *filp);
loff_t pcd_lseek (struct file *filp, loff_t off, int whence);
ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);

int check_permission(enum ePERMISSION dev_perm, int access_mode) {
    // if(dev_perm == RDWR)
    //     return 0;
    
    // if((dev_perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
    //     return 0;
    
    // if((dev_perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
    //     return 0;
    
    return INVALID_PERM;
}

int pcd_open (struct inode *inode, struct file *filp) {
    // int ret;
    // struct pcdev_private_data *pcdev_data;

    // /* extract the minor number */
    // int minor_number = MINOR(inode->i_rdev);
    // pr_info("minor access = %d\n", minor_number);

    // /* extract the cdev and the  container of cdev */
    // pcdev_data = container_of(inode->i_cdev, struct pcdev_private_data, cdev);

    // /* store the private data to be used in other file ops*/
    // filp->private_data = pcdev_data;

    // /* check permissions */
    // ret = check_permission(pcdev_data->perm ,filp->f_mode);
    
    // (!ret) ? pr_info("Open was successful!\n") : pr_info("Open was unsuccessful!\n");
    return 0;
}

int pcd_release (struct inode *inode, struct file *filp) {
    pr_info("Release was successful!\n");
    return 0;
}

loff_t pcd_lseek (struct file *filp, loff_t offset, int whence) {
    pr_info("lseek requested!\n");
    pr_info("Current file position = %lld \n", filp->f_pos);
    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
    // loff_t sz_total = 0;

    // switch (whence) {
    //     case SEEK_SET:
    //         if (offset > pcdev_data->sz || offset < 0) 
    //             return -EINVAL;
            
    //         filp->f_pos = offset;
    //         break;

    //     case SEEK_CUR:
    //         sz_total = filp->f_pos + offset;
    //         if (sz_total > pcdev_data->sz || offset < 0)
    //             return -EINVAL;

    //         filp->f_pos = sz_total;
    //         break;

    //     case SEEK_END:
    //         sz_total = pcdev_data->sz + offset;
    //         if (sz_total > pcdev_data->sz || offset < 0)
    //             return -EINVAL;

    //         filp->f_pos = sz_total;
    //         break;

    //     default:
    //         return -EINVAL; /* invalid whence value */
    // }

    // pr_info("Updated file position = %lld \n", filp->f_pos);

    // return filp->f_pos;
    return 0;
}

ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld \n", *f_pos);
    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

    // /* Check the count */
    // if ((count + *f_pos) > pcdev_data->sz) {
    //     /* update 'count' with current available buff-size to be read */
    //     count = pcdev_data->sz - *f_pos;
    // }

    // /* Copy to user */
    // if (copy_to_user(buff, pcdev_data->buffer+(*f_pos), count)) {
    //     return -EFAULT;
    // }

    // /* Update f_pos */
    // *f_pos += count;

    // pr_info("Number of bytes successfully read %zu \n", count);
    // pr_info("Updated file position = %lld \n", *f_pos);

    // /* Return successfully read bytes */
    // return count;
    return 0;
}

ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Write requested for %zu bytes \n", count);
    pr_info("Current file position = %lld \n", *f_pos);

    // struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

    // /* Check the count */
    // if(count == 0) {
    //     return -EINVAL;
    // }
    
    // if ((count + *f_pos) > pcdev_data->sz) {
    //     count = pcdev_data->sz - *f_pos;
    // }

    // /* Write on device buffer */
    // if(copy_from_user(pcdev_data->buffer+(*f_pos),buff, count)) {
    //     return -EFAULT;
    // }

    // /* Update file position */
    // *f_pos += count;

    // pr_info("Number of bytes successfully written %zu \n", count);
    // pr_info("Updated file position = %lld \n", *f_pos);

    // /* Return successfully written data */
    // return count;
    return 0;
}

/* file operations of the driver */
struct file_operations pcd_fops = {
    .open = pcd_open,
    .release = pcd_release,
    .llseek = pcd_lseek,
    .read = pcd_read,
    .write = pcd_write,
    .owner = THIS_MODULE
};

int pcd_platform_driver_probe (struct platform_device* dev) {
    pr_info("probe executed\n");
    return 0;
}

void pcd_platform_driver_remove(struct platform_device* dev) {
    pr_info("release executed\n");
}

struct platform_driver pcd_platform_driver = {
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
        .name = "pseudo-char-device"
    }
};

static int __init pcd_platform_driver_init (void) {
    pr_info("Driver loaded\n");
    int errCode;
    errCode = alloc_chrdev_region(&pcdrv_data.device_num_base, PCDEV_MINOR_BASE_NUMBER, MAX_DEVICES, "pcd_devices");

    if (errCode < 0) {
        pr_err("Device dynamic allocation failed!\n");
        return errCode;
        // goto out;
    }

    /* 2 create device class under /sys/class/ */
    pcdrv_data.class_pcd = class_create("pcd_class");
    if(IS_ERR(pcdrv_data.class_pcd)) {
        pr_err("Class creation failed!\n");
        errCode = PTR_ERR(pcdrv_data.class_pcd);
        // goto unreg_chrdev;
        unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
        return errCode;
    }

    /* Register a platform drver */
    platform_driver_register(&pcd_platform_driver);
    return 0;
}

static void __exit pcd_platform_driver_cleanup (void) {
    pr_info("Driver unloaded\n");
    /* 1. Unregister the platform driver */
    platform_driver_unregister(&pcd_platform_driver);

    /* 2. Class delete */
    class_destroy(pcdrv_data.class_pcd);

    /* 3. unregister device numbers for MAX_DEVICES */
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
}

module_init(pcd_platform_driver_init);
module_exit(pcd_platform_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ahmad.ADDAKHAKHNY");
MODULE_DESCRIPTION("A hello world kernel module example with n devices");
MODULE_INFO(board, "Beaglebone Black REV A5");
