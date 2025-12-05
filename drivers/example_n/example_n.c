#include<linux/module.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

/* MACROS */
#define NO_OF_DEVICES        4
#define BASE_MINOR_NUMBER    0
#define DEV_BUFFER_PCDEV1    1024
#define DEV_BUFFER_PCDEV2    512
#define DEV_BUFFER_PCDEV3    1024
#define DEV_BUFFER_PCDEV4    512

enum ePCDEV {
    PCDEV_1 = 0,
    PCDEV_2 = 1,
    PCDEV_3 = 2,
    PCDEV_4 = 3,
};

enum ePERMISSION {
    RDONLY = 0,
    WRONLY = 1,
    RDWR = 2
};

/* Device private data */
struct pcdev_private_data {
    char *buffer;
    unsigned sz;
    const char *serial_number;
    enum ePERMISSION perm;
    struct cdev cdev;
};

/* Driver private data */
struct pcdrv_private_data {
    int total_devices;
    dev_t device_number; // shall it be an array?
    struct class* class_pcd;
    struct device* pcd_device; // shall it be array?
    struct pcdev_private_data pcdev_data[NO_OF_DEVICES];
};

/* Pseudo device's memory */
char device_buffer_pcdev1[DEV_BUFFER_PCDEV1];
char device_buffer_pcdev2[DEV_BUFFER_PCDEV2];
char device_buffer_pcdev3[DEV_BUFFER_PCDEV3];
char device_buffer_pcdev4[DEV_BUFFER_PCDEV4];

struct pcdrv_private_data pcdrv_data = {
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
        [PCDEV_1] = {
            .buffer = device_buffer_pcdev1,
            .sz = DEV_BUFFER_PCDEV1,
            .serial_number = "PCDEV1XXXABC123",
            .perm = RDONLY
        },

        [PCDEV_2] = {
            .buffer = device_buffer_pcdev2,
            .sz = DEV_BUFFER_PCDEV2,
            .serial_number = "PCDEV2XXXABC123",
            .perm = WRONLY
        },

        [PCDEV_3] = {
            .buffer = device_buffer_pcdev3,
            .sz = DEV_BUFFER_PCDEV3,
            .serial_number = "PCDEV3XXXABC123",
            .perm = RDWR
        },

        [PCDEV_4] = {
            .buffer = device_buffer_pcdev4,
            .sz = DEV_BUFFER_PCDEV4,
            .serial_number = "PCDEV4XXXABC123",
            .perm = RDWR
        }
    }
};



/* Function prototypes */
int pcd_open (struct inode *inode, struct file *filp);
int pcd_release (struct inode *inode, struct file *filp);
loff_t pcd_lseek (struct file *filp, loff_t off, int whence);
ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);


int pcd_open (struct inode *inode, struct file *filp) {
    pr_info("Open was successful!\n");
    return 0;
}

int pcd_release (struct inode *inode, struct file *filp) {
    pr_info("Release was successful!\n");
    return 0;
}

loff_t pcd_lseek (struct file *filp, loff_t offset, int whence) {
    pr_info("lseek requested!\n");
    pr_info("Current file position = %lld \n", filp->f_pos);
#if 0
    loff_t sz_total = 0;
    switch (whence) {
        case SEEK_SET:
            if (offset > DEV_MEM_SIZE || offset < 0) 
                return -EINVAL;
            
            filp->f_pos = offset;
            break;

        case SEEK_CUR:
            sz_total = filp->f_pos + offset;
            if (sz_total > DEV_MEM_SIZE || offset < 0)
                return -EINVAL;

            filp->f_pos = sz_total;
            break;

        case SEEK_END:
            sz_total = DEV_MEM_SIZE + offset;
            if (sz_total > DEV_MEM_SIZE || offset < 0)
                return -EINVAL;

            filp->f_pos = sz_total;
            break;

        default:
            return -EINVAL; /* invalid whence value */
    }

    pr_info("Updated file position = %lld \n", filp->f_pos);

    return filp->f_pos;
#endif
    return -EINVAL;
}

ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld \n", *f_pos);
#if 0
    /* Check the count */
    if ((count + *f_pos) > DEV_MEM_SIZE) {
        /* update 'count' with current available buff-size to be read */
        count = DEV_MEM_SIZE - *f_pos;
    }

    /* Copy to user */
    if (copy_to_user(buff, &device_buffer[*f_pos], count)) {
        return -EFAULT;
    }

    /* Update f_pos */
    *f_pos += count;

    pr_info("Number of bytes successfully read %zu \n", count);
    pr_info("Updated file position = %lld \n", *f_pos);

    /* Return successfully read bytes */
    return count;
#endif
    return 0;
}

ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
#if 0
    pr_info("Write requested for %zu bytes \n", count);
    pr_info("Current file position = %lld \n", *f_pos);

    /* Check the count */
    if(count == 0) {
        return -EINVAL;
    } else if ((count + *f_pos) > DEV_MEM_SIZE) {
        return -ENOMEM;
    }

    /* Write on device buffer */
    if(copy_from_user(&device_buffer[*f_pos],buff, count)) {
        return -EFAULT;
    }

    /* Update file position */
    *f_pos += count;

    pr_info("Number of bytes successfully written %zu \n", count);
    pr_info("Updated file position = %lld \n", *f_pos);

    /* Return successfully written data */
    return count;
#endif
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

static int __init pcd_driver_init (void) {
#if 0
    int errCode = 0;
    /* 1. Dynamically allocate a device number */
    errCode = alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");
    if (errCode < 0) {
        pr_err("Device dynamic allocation failed!\n");
        goto out;
    }

    /* 2. initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);
    pcd_cdev.owner = THIS_MODULE;
    
    /* 3. register a device (cdev struture) with VFS */
    errCode = cdev_add(&pcd_cdev, device_number, 1);
    if (errCode < 0) {
        pr_err("Device registeration to VFS failed!\n");
        goto unreg_chrdev;
    }

    /* 4.1 create device class under /sys/class/ */
    class_pcd = class_create("pcd_class");
    if(IS_ERR(class_pcd)) {
        pr_err("Class creation failed!\n");
        errCode = PTR_ERR(class_pcd);
        goto cdev_del;
    }

    /* 4.2 populate device file info under /sys/class/ */
    pcd_device = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    if(IS_ERR(pcd_device)) {
        pr_err("Device file creation failed!\n");
        errCode = PTR_ERR(pcd_device);
        goto class_del;
    }

    pr_info("Device loading failed!\n");
    return 0;

class_del:
    device_destroy(class_pcd, device_number);
cdev_del:
    cdev_del(&pcd_cdev);
unreg_chrdev:
    unregister_chrdev_region(device_number, 1);
out:
    pr_err("Device file creation failed!\n");
    return errCode;
#endif
    return 0;
}

static void __exit pcd_driver_cleanup (void) {
#if 0
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);

    pr_info ("pcd cleanup was successful!\n");
#endif
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ahmad.ADDAKHAKHNY");
MODULE_DESCRIPTION("A hello world kernel module example with n devices");
MODULE_INFO(board, "Beaglebone Black REV A5");
