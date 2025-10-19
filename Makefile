# Root of the project
SRC_DIR := $(CURDIR)
CROSS_COMPILE := $(SRC_DIR)/sdk/sysroot//gcc-linaro-14.0.0-2023.06-x86_64_arm-linux-gnueabihf/bin
YOCTO_BUILD := $(CURDIR)/../BBB-Yocto-Build/build/tmp/work/beaglebone-poky-linux-gnueabi/linux-bb.org
KDIR=$(YOCTO_BUILD)/6.*+git/build

# BUILD_DIR := $(SRC_DIR)/builds/$(ARCH)
# INSTALL_DIR := $(SRC_DIR)/install
# INSTALL_PREFIX := $(INSTALL_DIR)/$(ARCH)
# CONAN_DIR := $(SRC_DIR)/external

ARCH=arm
