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
};
struct pcdrv_private_data pcdrv_data;

/* Function prototypes */
int pcd_platform_driver_probe (struct platform_device* dev);
int pcd_platform_driver_remove(struct platform_device* dev);
int check_permission(enum ePERMISSION dev_perm, int access_mode);
int pcd_open (struct inode *inode, struct file *filp);
int pcd_release (struct inode *inode, struct file *filp);
loff_t pcd_lseek (struct file *filp, loff_t off, int whence);
ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos);
ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos);

int check_permission(enum ePERMISSION dev_perm, int access_mode) {
    if(dev_perm == RDWR)
        return 0;
    
    if((dev_perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
        return 0;
    
    if((dev_perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
        return 0;
    
    return INVALID_PERM;
}

int pcd_open (struct inode *inode, struct file *filp) {
    int ret;
    struct pcdev_private_data *pcdev_data;

    /* extract the minor number */
    int minor_number = MINOR(inode->i_rdev);
    pr_info("minor access = %d\n", minor_number);

    /* extract the cdev and the  container of cdev */
    pcdev_data = container_of(inode->i_cdev, struct pcdev_private_data, cdev);

    /* store the private data to be used in other file ops*/
    filp->private_data = pcdev_data;

    /* check permissions */
    ret = check_permission(pcdev_data->pdata.perm ,filp->f_mode);
    
    (!ret) ? pr_info("Open was successful!\n") : pr_info("Open was unsuccessful!\n");
    return 0;
}

int pcd_release (struct inode *inode, struct file *filp) {
    pr_info("Release was successful!\n");
    return 0;
}

loff_t pcd_lseek (struct file *filp, loff_t offset, int whence) {
    pr_info("lseek requested!\n");
    pr_info("Current file position = %lld \n", filp->f_pos);
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;
    loff_t sz_total = 0;

    switch (whence) {
        case SEEK_SET:
            if (offset > pcdev_data->pdata.size || offset < 0) 
                return -EINVAL;
            
            filp->f_pos = offset;
            break;

        case SEEK_CUR:
            sz_total = filp->f_pos + offset;
            if (sz_total > pcdev_data->pdata.size || offset < 0)
                return -EINVAL;

            filp->f_pos = sz_total;
            break;

        case SEEK_END:
            sz_total = pcdev_data->pdata.size + offset;
            if (sz_total > pcdev_data->pdata.size || offset < 0)
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
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

    /* Check the count */
    if ((count + *f_pos) > pcdev_data->pdata.size) {
        /* update 'count' with current available buff-size to be read */
        count = pcdev_data->pdata.size - *f_pos;
    }

    /* Copy to user */
    if (copy_to_user(buff, pcdev_data->buffer+(*f_pos), count)) {
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

    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data*)filp->private_data;

    /* Check the count */
    if(count == 0) {
        return -EINVAL;
    }
    
    if ((count + *f_pos) > pcdev_data->pdata.size) {
        count = pcdev_data->pdata.size - *f_pos;
    }

    /* Write on device buffer */
    if(copy_from_user(pcdev_data->buffer+(*f_pos),buff, count)) {
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

int pcd_platform_driver_probe (struct platform_device* pdev) {
    pr_info("probe executed\n");
    int ret;
    // driver private data structure pointer
    struct pcdev_private_data *dev_data;

    // platform device data structure (substituteed by DT - next sessions)
    struct pcdev_platform_data *pdata;
    
    /* 1. Get the platform data*/
    // if platform data isn't found, dont procedd! this is developer POV.
    // pdata = (struct pcdev_platform_data*)pdev->dev.platform_data;
    pdata = (struct pcdev_platform_data*)dev_get_platdata(&pdev->dev);
    if(!pdata) {
        pr_info("No platform data available\n");
        ret = -EINVAL;
        goto out;
    }

    /* 2. Dynamically allocate memory for the device private data */
    dev_data = devm_kzalloc(&pdev->dev, sizeof(*dev_data), GFP_KERNEL);
    if(!dev_data) {
        pr_info("Can't allocate memory\n");
        ret = -EINVAL;
        goto out;
    }
    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;
    pr_info("Device serial number = %s\n", dev_data->pdata.serial_number);
    pr_info("Device size = %d\n", dev_data->pdata.size);
    pr_info("Device permission = %d\n", dev_data->pdata.perm);

    /* 3. Dynamically allocate memory for the device buffer using size info from the platform data*/
    dev_data->buffer = devm_kzalloc(&pdev->dev, dev_data->pdata.size, GFP_KERNEL);
    if(!dev_data->buffer) {
        pr_info("Can't allocate memory\n");
        ret = -EINVAL;
        goto dev_data_free;
    }

    /* 4. Get device number */
    dev_data->device_num = pcdrv_data.device_num_base + pdev->id;

    /* 5. Do cdev init and cdev add */
    cdev_init(&dev_data->cdev, &pcd_fops);
    dev_data->cdev.owner = THIS_MODULE;
    
    /* 4. register a device (cdev struture) with VFS */
    ret = cdev_add(&dev_data->cdev, dev_data->device_num, 1);
    if (ret < 0) {
        pr_err("Device registeration to VFS failed!\n");
        goto buffer_free;
    }

    /* 6. Create device file for the detected platform device */
    pcdrv_data.pcd_device = device_create(pcdrv_data.class_pcd, NULL, dev_data->device_num , NULL, "pcdev-%d", pdev->id +1);
    if(IS_ERR(pcdrv_data.pcd_device)) {
        pr_err("Device file creation failed!\n");
        ret = PTR_ERR(pcdrv_data.pcd_device);
        goto cdev_del;
    }
    // 7. Expose the device data to the remove method in order to de-allocate heap memory
    // pdev->dev.driver_data = dev_data;
    pcdrv_data.total_devices++;
    dev_set_drvdata(&pdev->dev, dev_data);
    pr_info("Device is detected\n");
    return 0;
    
    /* 8. Error handling */
cdev_del:
    cdev_del(&dev_data->cdev);
buffer_free:
    devm_kfree(&pdev->dev, dev_data->buffer);
dev_data_free:
    devm_kfree(&pdev->dev, dev_data);
out:
    pr_info("Device probe failed\n");
    return ret;
}

int pcd_platform_driver_remove(struct platform_device* pdev) {
    pr_info("release executed\n");
    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);
    /* 1. Remove a device that was created with device_create() */
    device_destroy(pcdrv_data.class_pcd, dev_data->device_num);
    /* 2. Remove a cdev entry from the system */
    cdev_del(&dev_data->cdev);
    pcdrv_data.total_devices--;

    return 0;
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
