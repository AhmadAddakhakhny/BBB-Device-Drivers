#include<linux/module.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

/* MACROS */
#define DEV_MEM_SIZE    512

/* Function prototypes */
int pcd_open (struct inode *inode, struct file *filp);
int pcd_release (struct inode *inode, struct file *filp);
loff_t pcd_lseek (struct file *filp, loff_t off, int whence);
ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);

/* Pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/* This holds the device number [major, minro] */
dev_t device_number;

/* cdev variable */
struct cdev pcd_cdev;

struct class* class_pcd;

struct device* pcd_device;

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
}

ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read requested for %zu bytes \n", count);
    pr_info("Current file position = %lld \n", *f_pos);

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
}

ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {

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
    /* 1. Dynamically allocate a device number */
    alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");

    /* 2. initialize the cdev structure with fops */
    cdev_init(&pcd_cdev, &pcd_fops);
    pcd_cdev.owner = THIS_MODULE;
    
    /* 3. register a device (cdev struture) with VFS */
    cdev_add(&pcd_cdev, device_number, 1);

    /* 4.1 create device class under /sys/class/ */
    class_pcd = class_create("pcd_class");

    /* 4.2 populate device file info under /sys/class/ */
    pcd_device = device_create(class_pcd, NULL, device_number, NULL, "pcd");

    pr_info("Module init was successful!\n");
    return 0;
}

static void __exit pcd_driver_cleanup (void) {

    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    cdev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);

    pr_info ("pcd cleanup was successful!\n");
}

module_init(pcd_driver_init);
module_exit(pcd_driver_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ahmad.ADDAKHAKHNY");
MODULE_DESCRIPTION("A hello world kernel module example");
MODULE_INFO(board, "Beaglebone Black REV A5");
