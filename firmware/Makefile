# Set verbosity
ifeq ($(V), 1)
Q =
else
Q = @
endif

# Commands
CC      = $(Q)arm-none-eabi-gcc
AS      = $(Q)arm-none-eabi-as
LD      = $(Q)arm-none-eabi-ld
AR      = $(Q)arm-none-eabi-ar
RM      = $(Q)rm
CPP     = $(Q)arm-none-eabi-cpp
SIZE    = $(Q)arm-none-eabi-size
STRIP   = $(Q)arm-none-eabi-strip -s
OBJCOPY = $(Q)arm-none-eabi-objcopy
OBJDUMP = $(Q)arm-none-eabi-objdump
PYTHON  = $(Q)python
MKDFU   = tools/dfu.py
PYDFU   = tools/pydfu.py
MKDIR   = $(Q)mkdir
ECHO    = $(Q)@echo
MAKE    = $(Q)make
CAT     = $(Q)cat

# Targets
FIRMWARE=minime

# Directories
TOP_DIR=$(shell pwd)
BUILD=$(TOP_DIR)/build
CMSIS_DIR=cmsis
STHAL_DIR=sthal
MINIME_DIR=minime
FW_DIR=$(TOP_DIR)/firmware/

# Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb3
else
CFLAGS += -O2 -ggdb3 -DNDEBUG
endif

# Enable debug printf
ifeq ($(DEBUG_PRINTF), 1)
CFLAGS += -DDEBUG_PRINTF
endif

# Enable stack protection
ifeq ($(STACK_PROTECTOR), 1)
CFLAGS += -fstack-protector-all -DSTACK_PROTECTOR
endif

# Compiler Flags
CFLAGS += -std=gnu99 -Wall -Werror -Warray-bounds -mthumb -nostartfiles -mabi=aapcs-linux -fdata-sections -ffunction-sections
CFLAGS += -DSTM32F042x6 -mcpu=cortex-m0 -mtune=cortex-m0 -mfloat-abi=soft
CFLAGS += -D__FPU_PRESENT=1 -D__VFP_FP__ -DUSE_USB_FS -DUSE_DEVICE_MODE -DUSE_USB_OTG_ID=0 -DHSE_VALUE=12000000 

CFLAGS += -I. -Iinclude
CFLAGS += -I$(TOP_DIR)/$(STHAL_DIR)/include/
CFLAGS += -I$(TOP_DIR)/$(CMSIS_DIR)/include/

# Linker Flags
LDFLAGS = -mcpu=cortex-m0 -mtune=cortex-m0 -mfloat-abi=soft -mabi=aapcs-linux -mthumb\
          -nostdlib -Wl,--gc-sections -Wl,-T$(MINIME_DIR)/stm32f0xx.ld

FIRM_OBJ += $(wildcard $(BUILD)/$(STHAL_DIR)/src/*.o)

#------------- MINIME Objects ----------------#
FIRM_OBJ += $(addprefix $(BUILD)/$(MINIME_DIR)/, \
	main.o                                  \
	string.o                                \
	stm32f0xx_it.o                          \
	)

FIRM_OBJ += $(addprefix $(BUILD)/$(CMSIS_DIR)/src/,\
	system_stm32f0xx.o                          \
	startup_stm32f042x6.o                       \
	)

###################################################
#Export Variables
export Q
export CC
export AS
export LD
export AR
export SIZE
export OBJCOPY
export OBJDUMP
export MKDIR
export ECHO
export CFLAGS
export LDFLAGS
export TOP_DIR
export BUILD
export STARTUP
###################################################
all: $(FIRMWARE)

$(BUILD):
	$(MKDIR) -p $@

FIRMWARE_OBJS:
	$(MAKE)  -C $(CMSIS_DIR)          BUILD=$(BUILD)/$(CMSIS_DIR) CFLAGS="$(CFLAGS) -fno-strict-aliasing"
	$(MAKE)  -C $(STHAL_DIR)          BUILD=$(BUILD)/$(STHAL_DIR)
	$(MAKE)  -C $(MINIME_DIR)         BUILD=$(BUILD)/$(MINIME_DIR)

# This target generates the main/app firmware image located at 0x08010000
$(FIRMWARE): FIRMWARE_OBJS | $(BUILD)
	$(CC) $(LDFLAGS) $(FIRM_OBJ) -o $(FW_DIR)/$(FIRMWARE).elf -lgcc
	$(OBJCOPY) -Obinary $(FW_DIR)/$(FIRMWARE).elf $(FW_DIR)/$(FIRMWARE).bin
	$(PYTHON) $(MKDFU) -b 0x08000000:$(FW_DIR)/$(FIRMWARE).bin $(FW_DIR)/$(FIRMWARE).dfu
	$(SIZE) $(FW_DIR)/$(FIRMWARE).elf

size:
	$(SIZE) --format=SysV $(FW_DIR)/$(FIRMWARE).elf

clean:
	$(RM) -fr $(BUILD)

flash::
	dfu-util -D $(FW_DIR)/$(FIRMWARE).dfu -d 0483:df11 -a 0 -R
