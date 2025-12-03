#include<linux/module.h>
#include<linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#define DEV_MEM_SIZE    512

/* Pseudo device's memory*/
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
    pr_info("Close was successful!\n");
    return 0;
}

loff_t pcd_lseek (struct file *filp, loff_t off, int whence) {
    pr_info("lseek was successful!\n");
    return 0;
}

ssize_t pcd_read (struct file *filp, char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Read was successful!\n");
    return 0;
}

ssize_t pcd_write (struct file *filp, const char __user *buff, size_t count, loff_t *f_pos) {
    pr_info("Rrite was successful!\n");
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
    // 1. Dynamically allocate a device number
    alloc_chrdev_region(&device_number, 0, 1, "pcd_devices");

    // 2. initialize the cdev structure with fops
    cdev_init(&pcd_cdev, &pcd_fops);
    pcd_cdev.owner = THIS_MODULE;
    
    // 3. register a device (cdev struture) with VFS
    cdev_add(&pcd_cdev, device_number, 1);

    // 4.1 create device class under /sys/class/
    class_pcd = class_create("pcd_class");

    // 4.2 populate device file info under /sys/class/
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
