// #include<linux/module.h>

// static int __init my_example_init (void) {
//     pr_info("Hello, World from LKM!\n");
//     return 0;
// }

// static void __exit my_example_exit (void) {
//     pr_info ("Good bye, World from LKM!\n");
// }

// module_init(my_example_init);
// moudle_exit(my_example_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Ahmad.ADDAKHAKHNY");
// MODULE_DESCRIPTION("A hello world kernel module example");
// MODULE_INFO(board, "Beaglebone Black REV A5");

#include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/init.h>

static int __init my_example_init(void) {
    printk(KERN_INFO "Example module loaded\n");
    return 0;
}

static void __exit my_example_exit(void) {
    printk(KERN_INFO "Example module unloaded\n");
}

module_init(my_example_init);
module_exit(my_example_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Example BBB Device Driver");
