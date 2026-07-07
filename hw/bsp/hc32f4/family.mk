HC32_SDK = hw/mcu/hdsc/sdk-csp-hc32f4
HC32_CMSIS = $(HC32_SDK)/libraries/cmsis
HC32_DEVICE = $(HC32_CMSIS)/Device/HDSC/hc32f4xx
HC32_DDL = $(HC32_SDK)/libraries/hc32f460_ddl/hc32_ll_driver

include $(TOP)/$(BOARD_PATH)/board.mk

CPU_CORE ?= cortex-m4

CFLAGS += \
  -DCFG_TUSB_MCU=OPT_MCU_HC32F4 \
  -DHC32F460 \
  -DUSE_DDL_DRIVER

CFLAGS += \
  -Wno-redundant-decls \
  -Wno-unused-parameter \
  -Wno-error=cast-align \
  -Wno-error=conversion \
  -Wno-error=null-dereference \
  -Wno-error=redundant-decls \
  -Wno-error=sign-conversion \
  -Wno-error=unused-parameter

CFLAGS_SKIP += -Wnull-dereference -Wredundant-decls

LDFLAGS += \
  -nostartfiles \
  --specs=nosys.specs \
  --specs=nano.specs

SRC_C += \
  src/portable/synopsys/dwc2/dcd_dwc2.c \
  src/portable/synopsys/dwc2/hcd_dwc2.c \
  src/portable/synopsys/dwc2/dwc2_common.c \
  $(HC32_DEVICE)/Source/system_hc32f460.c \
  $(HC32_DDL)/src/hc32_ll.c \
  $(HC32_DDL)/src/hc32_ll_clk.c \
  $(HC32_DDL)/src/hc32_ll_efm.c \
  $(HC32_DDL)/src/hc32_ll_fcg.c \
  $(HC32_DDL)/src/hc32_ll_gpio.c \
  $(HC32_DDL)/src/hc32_ll_interrupts.c \
  $(HC32_DDL)/src/hc32_ll_pwc.c \
  $(HC32_DDL)/src/hc32_ll_sram.c \
  $(HC32_DDL)/src/hc32_ll_usart.c \
  $(HC32_DDL)/src/hc32_ll_utility.c

SRC_S += $(HC32_DEVICE)/Source/GCC/startup_hc32f460.S

INC += \
  $(TOP)/$(FAMILY_PATH) \
  $(TOP)/$(BOARD_PATH) \
  $(TOP)/$(HC32_CMSIS)/Include \
  $(TOP)/$(HC32_DEVICE)/Include \
  $(TOP)/$(HC32_DDL)/inc

LD_FILE ?= $(HC32_SDK)/linkscripts/HC32F460xE/link.ld

flash: flash-pyocd
