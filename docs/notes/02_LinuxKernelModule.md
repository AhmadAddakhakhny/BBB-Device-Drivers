
##### Why LKM?
> it has the ability to extend the kernel functionality/features such as security
##### What are the types of linux kernel modules?
> device drivers, file system drivers.  
##### What is the difference between static and dynamic LKMs?
> **Static(y):** it's a part of the kernel image, and that increases the size of it, and occupies the memory permanently during run time.  
> **Dynamic(m):** It's not a part of the kernel image, and compiled and linked separately to produce .ko files. and user-space programs can deal with it. i.e insmod, modprobe, rmmod. with extension .ko.  

##### user-space vs kernel-space?
> user-space: Restricted mode - user level code  
> kernel-space: privilleged mode - kernel level code  
---
##### How would a minimal hello world kernel module be structured?
> Header section, Code section, Registeration section, Description section

###### Header sectiond?
> #inculde <linux/module.h> - every LKM should include it. // kernel header path LINUX_SRC/include/linux  
> P.S. Never include user-space header!
---
###### Code section?
> Mainly there are two entery point functions.
```c
/* This is module initialization entry point */
static int __init my_kernel_module_init(void){
    // Kernel's printf
    pr_info("Hello, World!\n");
    return 0;
    // Explain:
    // Must return a value; 0 for sucess, otherwise module won't be loaded into the kernel.
    // we shall consider it the main() or constructor() of this module
    // static module: it executes during boot time.
    // dynamic module: it exevutes during insertion
}

/* This is module clean-up entry point */
static void __exit my_kernel_module_exit(void){
    // Kernel's printf
    pr_info("Good bye, World!\n");
    // Description:
    // this function executes when the module is removed.
    // since you can't remove static modules, it executes only with dynamic modules (rmmod)
    // kenel build system will remove this function if module is configured static.
    // this function should undo what init function has done. (destructor)
}
```
> static keyword used to let functions private to that specific LKM. and can't being invoked by other modules.  
---
###### Function section attributes __init, __exit?
> they are compiler directive, which directs the compiler to keep data or code in an output section called ".init" or ".exit"
```c
#define __init           __section(.init.text)     // compiler directive
#define __initdata       __section(.init.data)     // compiler directive
#define __initconst      __section(.init.rodata)   // compiler directive
#define __exit           __section(.exit.text)     // compiler directive
```
---
###### Registeration section?
> there are registeration MACRO for module entry points.
```c
    // Register module's init function
    module_init(my_kernel_module_init);

    // Register module's exit function
    moudle_exit(my_kernel_module_exit);
```
---
###### Module description section?
> it's nothing but a metadata would be secriping the kernel module.
```c
    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("Ahmad.ADDAKHAKHNY");
    MODULE_DESCRIPTION("A hello world kernel module example");

```
---
###### how to know if a module is static or dynamic inside the machine?
>

---
### How to build a linux kernel module?
> Kernel module can be built in 2 ways:
> > 1. Statically linked against the kernel image.
> > 2. Dynamically loadable. (mostly will be dealing with this type)

##### How to build a dynamically loadable kernel module?
> In tree module (internal to the linux kernel tree)  
> Out of tree module (external to the linux kernel tree) - this methods taints the kernel and we can safely ignore this warning!  
> Modules are being built by "kbuild" which is the build system used by the Linux kernel.  
> Modules must use "kbuild" to stay compatible with changes in the build infrastructure and to pick up the right flags to GCC.
> Kbuild: holds the compiler config flags so developers don't have to set them.  
> To build external modules, you must have a prebuilt kernel source available that contains the configuration and header files used in the build.  

##### Out of tree vs in tree kernel module?
> LKM that aren't officially signoff by linux.org are called out of tree modules, otherwise in tree modules.

##### The command used to build an external module is?
```bash
    $ make  -C <path/to/linux/kernel/tree> M=<path/to/external/module> [target]
```
> P.S. It's a must to trigger the top-level-Makefile (linux kernel source tree), that takes place with this option -C --- "make -C". which means go to the following path and execute the make target over there and then the top-level-Makefile would eventually be directed to the local Makefile. [top leve Makefile] -> [local Makefile]  
> [target]: it could be one of the following (modules, modules_install, clean, help)  

##### Steps to create a self contained LDD project?
> 0) Source the Yocto SDK (important)  
```bash
source ./sdk/environment-setup-armv7at2hf-neon-poky-linux-gnueabi
# verify compiler is available
which arm-poky-linux-gnueabihf-gcc || which arm-poky-linux-gnueabi-gcc
```
> 1) Copy the kernel source into the repo (KDIR/source)
```bash
mkdir -p KDIR/source
rsync -av --delete ../BBB-Yocto-Build/build/tmp/work-shared/beaglebone/kernel-source/ KDIR/source/
```

> 2) Copy the kernel build artifacts from the Yocto build
```bash
mkdir -p KDIR/out
YOCTO_BUILD=../BBB-Yocto-Build/build/tmp/work/beaglebone-poky-linux-gnueabi/linux-bb.org/6.12.34+git/build
# copy .config into KDIR/out (so the out tree has the same config)
cp -v ${YOCTO_BUILD}/.config KDIR/out/ || echo "Warning: .config not found in Yocto build"

# copy generated headers (if present) into out/include/generated
mkdir -p KDIR/out/include
rsync -av --delete ${YOCTO_BUILD}/include/generated/ KDIR/out/include/generated/ || echo "Warning: include/generated not found"

# copy Module.symvers into the out tree
cp -v ${YOCTO_BUILD}/Module.symvers KDIR/out/ || echo "Warning: Module.symvers not found in Yocto build"
```

> 3) Prepare the out-of-tree build directory (KDIR/out)
```bash
cd KDIR/source

# ensure you're using the cross toolchain prefix (short form, environment has it)
# we use the prefix without the trailing basename because the env provides that in PATH
CROSS_PREFIX=arm-poky-linux-gnueabihf-   # or arm-poky-linux-gnueabi- depending on SDK

# run non-interactive olddefconfig in the *output* tree (O=)
make O=../out ARCH=arm CROSS_COMPILE=${CROSS_PREFIX} olddefconfig

# prepare the output tree (generates auto.conf, autoconf.h etc in KDIR/out)
make O=../out ARCH=arm CROSS_COMPILE=${CROSS_PREFIX} prepare modules_prepare

cd ../../
```

> 4) Verify out tree is ready  

> 5) Build your module out-of-tree  
```bash
Summary (short)

source SDK.

rsync kernel source into KDIR/source.

copy .config, include/generated/ (optional), and Module.symvers from Yocto into KDIR/out.

run make -C KDIR/source O=KDIR/out olddefconfig and make -C KDIR/source O=KDIR/out prepare modules_prepare.

build out-of-tree: make -C KDIR/source O=KDIR/out M=drivers/yourmod ARCH=arm CROSS_COMPILE=arm-...- modules

copy resulting .ko from KDIR/out/... to your builds/ if desired.
```

##### How to build for x86?
> check the kernel source and header location $ uname -r => 6.8.0-86-generic
> lib/modules/6.8.0-86-generic
> you should build your module against this directory
> make -C /lib/modules/6.8.0-86-generic/build M=${PWD} modules
> make -C /lib/modules/6.8.0-86-generic/build M=${PWD} clean

##### Copy x86 headers, resolving symlinks (important!)
> $cp -aL /usr/src/linux-headers-$(uname -r)/* KDIR/x86/

##### How to insert/remove a module?
> sudo insmod main.ko  
> sudo rmmod main.ko  