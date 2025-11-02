# Debugging with printk()

> synatx: it's almost same as printf() function in userspace.  
> the log-message will be printed on the kernel-ring-buffer and sometimes on the console depends on the log level.

##### What is kernel log level?
> logs shall be defined with a priority level 0[highest], 7[lowest]:  
```bash
Level 	Name	        Description
0	    KERN_EMERG	    System is unusable
1	    KERN_ALERT	    Action must be taken immediately
2	    KERN_CRIT	    Critical conditions
3	    KERN_ERR	    Error conditions
4	    KERN_WARNING	Warning conditions
5	    KERN_NOTICE	    Normal but significant conditions
6	    KERN_INFO	    Informational messages
7	    KERN_DEBUG	    Debug-level messages
```

##### How to invoke printk()?
> 1. without log-level assignment .. printk("Hello LDD\n");  
> 2. with log-level assignment .. printk(KERN_INFO"Hello LDD\n");  
P.S. the default log-level is 4 in case of no assignment!

##### What is the default level of printk and how to change it?
> printk() is a macro-like-function and in default assigned with log-level 4.  
> it can be modified to another default by means of menuconfig.  
```bash
# 1. Go to linux source tree and run:
make ARCH=$(ARCH) menuconfig
select kernel hacking
select printk and dmesg options
adjust Default message log level (1-7)
```

##### What is the default consol log-level?
> it decides either the log-info be printed directly on the consol or should be placed in the dmesg only.  
> if the log-level higher than the consol level, the log will be printed on the consol directly.  
```bash
# Go to linux source tree and run:
make ARCH=$(ARCH) menuconfig
select kernel hacking
select printk and dmesg options
adjust Default consol log level (1-7)
```