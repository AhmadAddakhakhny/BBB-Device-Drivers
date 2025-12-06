# Platform bus, devices and drivers

### 1. What is a platform devie?
> it's a device that can't be self discovered by the kernel.

### 2. What is a platform driver?
> it's a driver that deals with a platform device and platform driver could be char, block, or network driver.   

### 3. What is a platform bus?
1. it's  pseudo bus or virtual bus. it doesn't have any physical existence. (it's a linux terminology)
2. it is a logical place holder or a container that contains all the undiscoverable devices on the SoC that the kernel must be told about manually via DT.
2. it's used to interact with interfaces that don't have auto dicoverable and hot plugging capability.
3. Remark: the PC has a standard bus called IPC which it's a bus performs the interconnect between the CPU and the peripheral and capable to perform self discovery.
> Remark: SoC peripherals don't need to be hot pluggable as they are permenent on the chip not as the PC (you can add/remove components).

### 4. What device information that shall Kernel be aware of?
1. Memory or I/O mapped base address
2. IRQ number
3. DMA channel
4. PIN config ... etc.

### 5. How to add platform device info to the kernel?
> During kernel boot: using device tree blob

### 6. What is Device tree?
1. DT was created by **Open Firmware** then concept was adopted by linux org.
2. It's used to avoid hardcoding for the chips inside the kernel.

### 7. Show BBB platform peripherals/controllers??
![alt text](img/01_image.png)

### 8. Show the BBB pltform peripherals/controllers firmware dirvers?
| Peripheral                      | Location / Driver Path                                   | Description                                           |
|---------------------------------|-----------------------------------------------------------|-------------------------------------------------------|
| SPI                             | drivers/spi/spi-omap2-mcspi.c                            | OMAP2 McSPI controller driver                         |
| I2C                             | drivers/i2c/busses/i2c-omap.c                            | TI OMAP I2C master mode driver                        |
| USB OTG                         | drivers/usb/musb/musb_am335x.c                           | MUSB controller                                       |
| CAN (C_CAN controller)          | drivers/net/can/c_can/c_can_platform.c                   | Platform CAN bus driver for Bosch C_CAN controller    |
| MMC                             | drivers/mmc/host/omap_hsmmc.c                            | Driver for OMAP2430/3430 MMC controller               |
| GPIO                            | drivers/gpio/gpio-omap.c                                 | Support functions for OMAP GPIO                       |
| UART                            | drivers/tty/serial/8250/8250_omap.c                      | 8250-core based driver for the OMAP internal UART     |
| LCD controller                  | drivers/gpu/drm/tilcdc/tilcdc_drv.c                      | LCDC DRM driver, based on da8xx-fb                    |
| Touch screen controller         | drivers/input/touchscreen/ti_am335x_tsc.c                | TI Touch Screen driver                                |

