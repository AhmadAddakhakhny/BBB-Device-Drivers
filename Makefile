#----------------------------------------
# Central Makefile for BBB-Device-Drivers
#----------------------------------------

# Root of the project
SRC_DIR := $(CURDIR)
ARCH := arm
DD := examples
# Cross-compiler from Yocto SDK
CROSS_COMPILE := $(SRC_DIR)/sdk/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-

# Self-contained kernel build directory
KDIR := $(SRC_DIR)/KDIR

# Build and install directories
BUILD_DIR := $(SRC_DIR)/builds/$(ARCH)
INSTALL_DIR := $(SRC_DIR)/install
INSTALL_PREFIX := $(INSTALL_DIR)/$(ARCH)

# Driver source directory
DRIVER_DIR := $(SRC_DIR)/drivers/$(DD)

# Default target: build the module
all:
	@echo "Building BBB device module..."
	@mkdir -p $(BUILD_DIR)
	$(MAKE) -C $(KDIR) M=$(DRIVER_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) O=$(BUILD_DIR) modules

# Clean target
clean:
	@echo "Cleaning module build..."
	$(MAKE) -C $(KDIR) M=$(DRIVER_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) O=$(BUILD_DIR) clean
	@rm -rf $(BUILD_DIR)

# Install the module
install: all
	@echo "Installing module to $(INSTALL_PREFIX)..."
	@mkdir -p $(INSTALL_PREFIX)
	@cp -v $(BUILD_DIR)/chardev.ko $(INSTALL_PREFIX)/



# ---------------------------In case of sync. KDIR with BBB-Yocto-Build---------------------------#
# Uncomment and adjust KDIR_SRC to use
# KDIR_SRC := ../BBB-Yocto-Build/build/tmp/work/beaglebone-poky-linux-gnueabi/linux-bb.org/6.*+git/build/
# sync-kernel:
# 	rsync -av --delete $(KDIR_SRC)/ $(KDIR)/