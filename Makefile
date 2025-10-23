#----------------------------------------
# Central Makefile for BBB-Device-Drivers
#----------------------------------------

SRC_DIR := $(CURDIR)
ARCH := arm
DD := example

CROSS_COMPILE := $(SRC_DIR)/sdk/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-

# Kernel source (self-contained)
KDIR := $(SRC_DIR)/KDIR/source

# Driver source directory
DRIVER_DIR := $(SRC_DIR)/drivers/$(DD)

# Build output (optional)
BUILD_DIR := $(SRC_DIR)/builds/$(ARCH)/$(DD)
INSTALL_DIR := $(SRC_DIR)/install
INSTALL_PREFIX := $(INSTALL_DIR)/$(ARCH)


# Default: build module
all: prep
	@echo "Building BBB device module..."
	@mkdir -p $(BUILD_DIR)
	make -C $(KDIR) M=$(PWD)/drivers/example O=$(BUILD_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) INSTALL_MOD_PATH=$(BUILD_DIR) modules

# Ensure kernel source is prepared
prep:
	@if [ ! -f $(KDIR)/include/config/auto.conf ]; then \
		echo "Preparing kernel source..."; \
		$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) olddefconfig; \
		$(MAKE) -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) prepare modules_prepare; \
	else \
		echo "Kernel source already prepared."; \
	fi

# Clean module build
clean:
	@echo "Cleaning module build..."
	$(MAKE) -C $(KDIR) M=$(DRIVER_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	@rm -rf $(BUILD_DIR)

# Install the module
install: all
	@echo "Installing module to $(INSTALL_PREFIX)..."
	@mkdir -p $(INSTALL_PREFIX)
	@cp -v $(DRIVER_DIR)/example.ko $(INSTALL_PREFIX)/
