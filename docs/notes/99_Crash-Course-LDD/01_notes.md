
 2. What is a Platform Device?
 A Platform Device represents a device that resides on the platform bus. It is usually:

Memory-mapped I/O (MMIO)
Integrated into the SoC
Not discoverable dynamically (unlike PCI/USB)
It is represented in code by the **struct platform_device**
platform_device_register(&my_platform_device); // to register it into the kernel.

What is a platform driver?
platform_device: Represents the hardware. It holds the static information about the device: its name, the memory regions (resources) it uses, its IRQ numbers, etc. In the modern Device Tree world, this structure is automatically created by the kernel when it parses a matching node in the Device Tree.
platform_driver: Represents the software that controls the hardware. This is the driver you write. It contains the probe/remove functions and a way to identify which platform_device it should manage.

### Matching Process: How Device & Driver Bind?
The platform_driver and platform_device are matched via the name field.
In Device Tree systems, the driver matches the compatible string via of_match_table.

### what are the functions are being invoked at loading and removing a driver?
.propbe = my_prope, // this function being invoked after matching
.remove = my_remove
---
### struct platform_device & platform_device_register
#### 1. struct platform_device





### Write a simple out of tree LDD?
```bash
SUMMARY = ""
DESCRIPTION = ""
LICENSE = ""
# use the kernel build system
inherit module
SRC_URI = " file://Makefile \
            file://led_gpio22.c \
            "
S = "${WORKDIR}"
PROVIDES:{PN} += "led_gpio22"
```
---

### What is device tree?
> it's a data structure the kernel reads at boot time. it describes what hardware exists on the system.
> it shows the peripheral location(address), interrupt number, and compatible string
🔧1. What is the Platform Bus?
The Platform Bus is a virtual bus in the Linux device model. it  is a framework that connects platform devices with platform drivers through device tree.



### Where is the location of the device tree?
/arch/arm/boot/dts/broadcom

### Device tree structure?
1. it's comosed out of nodes, at leas the root node. the dts shall include other dtsi
2. the device tree base of the SoC provide default configurations for example gpio-controller, to configure the pins registers with the reuqired multiplexing info.
3. the base device tree file shall be patched via device tree overlays the override takes place using & operator.

### How to compile the device tree?
1. via dtc as it takes dts and output dto
### How to apply device tree overlays via yocto and manual?
1. it can be defined in the /boot/config.txt


### if insmod applied to a LDD but no node defined for it, what happen?
> kernel would load it but won't make it active unless the device is inserted as well.

### What does Kobject, Kset, Kobj_type do?
> a fundamental block in linux kernel to represent an onject inside the kernel by exposing it to sysfs
---
# Steps to write a GPIO platform driver
> GPIO module -> GPIO platform driver (consumer)  
1. write your device tree overlays to configure the pins
    1. pin number = pin name
    1. configure as input/outpu
    1. if input .. decide pull-up or pull-down
```bash
gpio14_pins: gpio14_pinns {
brcm, pins = <14>;
brcm,function = <1>;    /* 1 = Output */
brcm,pull = <0>;        /* 0 = No pull-up / pull-down */
};
```
2. implement the gpio driver (match, prope, remove)
3. implement sysfs related attributes value_show, value_store, and create their kobject to be exposed to user space
4. provide direction file to show whether the pin out or input, and value to put 5 the value to be written