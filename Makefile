#----------------------------------------
# Central Makefile for BBB-Device-Drivers
#----------------------------------------

SRC_DIR := $(CURDIR)
ARCH := arm
DD := example

# Cross compiler from SDK
CROSS_COMPILE := $(SRC_DIR)/sdk/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-

# Kernel source and build output (Yocto export)
KDIR_SRC := $(SRC_DIR)/KDIR/source
KDIR_OUT := $(SRC_DIR)/KDIR/out

# Driver source directory
DRIVER_DIR := $(SRC_DIR)/drivers/$(DD)

# Module build and install staging directories
BUILD_DIR := $(SRC_DIR)/builds/$(ARCH)/$(DD)
INSTALL_DIR := $(SRC_DIR)/install
INSTALL_PREFIX := $(INSTALL_DIR)/$(ARCH)


# Default: build module
all: prep
	@echo "=============================================="
	@echo " Building BBB device module (out-of-tree)..."
	@echo " Source : $(DRIVER_DIR)"
	@echo " Output : $(BUILD_DIR)"
	@echo " Kernel : $(KDIR_SRC)"
	@echo "=============================================="
	@mkdir -p $(BUILD_DIR)
	# Copy sources into the out-of-tree build directory
	rsync -a --delete $(DRIVER_DIR)/ $(BUILD_DIR)/
	# Build the module out-of-tree
	$(MAKE) -C $(KDIR_SRC) O=$(KDIR_OUT) \
		M=$(BUILD_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# Ensure kernel source is prepared
prep:
	@if [ ! -f $(KDIR_OUT)/include/config/auto.conf ]; then \
		echo "Preparing kernel source..."; \
		mkdir -p $(KDIR_OUT); \
		$(MAKE) -C $(KDIR_SRC) O=$(KDIR_OUT) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) olddefconfig; \
		$(MAKE) -C $(KDIR_SRC) O=$(KDIR_OUT) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) prepare modules_prepare; \
	else \
		echo "Kernel source already prepared."; \
	fi

# Clean module build
clean:
	@echo "Cleaning module build..."
	$(MAKE) -C $(KDIR_SRC) O=$(KDIR_OUT) M=$(BUILD_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	@rm -rf $(BUILD_DIR)

# Install the module (to self-contained install dir)
install: all
	@echo "Installing module to $(INSTALL_PREFIX)..."
	@mkdir -p $(INSTALL_PREFIX)
	@cp -v $(BUILD_DIR)/*.ko $(INSTALL_PREFIX)/

.PHONY: all prep clean install
