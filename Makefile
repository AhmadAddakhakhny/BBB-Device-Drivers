#----------------------------------------
# Central Makefile for BBB-Device-Drivers
#----------------------------------------

SRC_DIR := $(CURDIR)
DD := example

# Default architecture
ARCH ?= arm

# Cross compiler (only for ARM)
CROSS_COMPILE := $(SRC_DIR)/sdk/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-

# Kernel source directories
KDIR_SRC := $(SRC_DIR)/KDIR/arm/source
KDIR_OUT := $(SRC_DIR)/KDIR/arm/out

# kernel source (for x86)
KDIR_HOST := $(SRC_DIR)/KDIR/x86

# Driver source directory
DRIVER_DIR := $(SRC_DIR)/drivers/$(DD)

# Module build and install staging directories
BUILD_DIR := $(SRC_DIR)/builds/$(ARCH)/$(DD)
INSTALL_DIR := $(SRC_DIR)/install
INSTALL_PREFIX := $(INSTALL_DIR)/$(ARCH)

# Select kernel directory and cross compiler based on ARCH
ifeq ($(ARCH),arm)
    KDIR := $(KDIR_SRC)
    CC := $(CROSS_COMPILE)
    ODIR := $(KDIR_OUT)
else
    # x86 host build
    KDIR := $(KDIR_HOST)
    CC :=
    ODIR := $(KDIR_HOST)  # Use host directory itself for O=
endif

# Default: build module
all: prep
	@echo "=============================================="
	@echo " Building BBB device module (out-of-tree)..."
	@echo " Architecture : $(ARCH)"
	@echo " Source       : $(DRIVER_DIR)"
	@echo " Output       : $(BUILD_DIR)"
	@echo " Kernel       : $(KDIR)"
	@echo "=============================================="
	@mkdir -p $(BUILD_DIR)
	# Copy sources into the out-of-tree build directory
	rsync -a --delete $(DRIVER_DIR)/ $(BUILD_DIR)/
	# Build the module out-of-tree
	$(MAKE) -C $(KDIR) O=$(ODIR) M=$(BUILD_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CC) modules

# Ensure kernel source is prepared
prep:
ifeq ($(ARCH),arm)
	@if [ ! -f $(ODIR)/include/config/auto.conf ]; then \
		echo "Preparing ARM kernel source..."; \
		mkdir -p $(ODIR); \
		$(MAKE) -C $(KDIR) O=$(ODIR) ARCH=$(ARCH) CROSS_COMPILE=$(CC) olddefconfig; \
		$(MAKE) -C $(KDIR) O=$(ODIR) ARCH=$(ARCH) CROSS_COMPILE=$(CC) prepare modules_prepare; \
	else \
		echo "ARM kernel source already prepared."; \
	fi
else
	@echo "(x86) kernel: assuming headers in $(KDIR) are ready"
endif

# Clean module build
clean:
	@echo "Cleaning module build..."
	$(MAKE) -C $(KDIR) O=$(ODIR) M=$(BUILD_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CC) clean
	@rm -rf $(BUILD_DIR)

# Install the module (to self-contained install dir)
install: all
	@echo "Installing module to $(INSTALL_PREFIX)..."
	@mkdir -p $(INSTALL_PREFIX)
	@cp -v $(BUILD_DIR)/*.ko $(INSTALL_PREFIX)/

.PHONY: all prep clean install
