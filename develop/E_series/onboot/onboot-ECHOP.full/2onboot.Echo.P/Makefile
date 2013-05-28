# -*- c -*- ********************************************************************
# Copyright (c) 2010 Samsung Electronics, Inc.
# All rights reserved.
#
# This software is the confidential and proprietary information
# of Samsung Electronics, Inc("Confidential Information"). You
# Shall not disclose such Confidential Information and shall use
# it only in accordance with the terms of the license agreement
# you entered into Samsung.
#*******************************************************************************
# System        : Echo 
# Module        : BOOT
#
# File Name     : Makefile
# Revision      : 
# Date Created  : 2011.06.24
# Date Modified : 
# Author        : SW2 System Platform
# Last Modified : 
#*******************************************************************************
# Description   : 
#
#*******************************************************************************
# Notes
#
#*******************************************************************************
# History
# 
#      
#*******************************************************************************/

#===============================================================================
# TARGET
#-------------------------------------------------------------------------------
TARGET_NAME	= onboot
LD_FILE = onboot.lds

#===============================================================================
# DIRECTORY CONFIGURATION
#-------------------------------------------------------------------------------
ECHO_B_BASE_D = echo
#===============================================================================

#===============================================================================
# MACHINE CONFIGURATION FOR ECHO 
#-------------------------------------------------------------------------------
CROSS = arm-v7a8v2r2-linux-gnueabi-
TEXT_BASE = 0x47c00000	# onboot base addr

#===============================================================================
# TOOL CONFIGURATION
#-------------------------------------------------------------------------------
CC	= $(CROSS)gcc #-ffunction-sections -fdata-sections
AS	= $(CROSS)gcc 
LD	= $(CROSS)ld
OC	= $(CROSS)objcopy
OD	= $(CROSS)objdump
RE	= $(CROSS)readelf
NM	= $(CROSS)nm

OPTFLAGS = -Os -g -mabi=aapcs #-Wall 

INC_PATH = -I./include/	\
			-I./$(ECHO_B_BASE_D)/include/ \
			-I./sdp -I./sp

CFLAGS = $(OPTFLAGS)
CFLAGS += -DTEXT_BASE=$(TEXT_BASE)
CFLAGS += -DCONFIG_SDP_MMC_OP
CFLAGS += -DFAST_LOGO

#ifeq ($(mmu),y)
#CFLAGS += -DUSE_MMU
#endif

ifeq ($(cpu),)
    cpu = 1000
endif

CFLAGS += -DFREQ_ARM=$(cpu)

ifeq ($(smp_config),)
    smp_config = SMP
endif

CFLAGS += -DCONFIG_$(smp_config)

ifeq ($(board_type),)
    board_type = DV
endif

CFLAGS += -DBOARD_$(board_type)

ifeq ($(mem_map),)
    mem_map = 1536
endif

CFLAGS += -DMEMORY_MAP_$(mem_map)MB


ifeq ($(dev),n)
CFLAGS += -DSILENT_KERNEL
endif

ifeq ($(country_jp), y)
CFLAGS += -DCOUNTRY_JP
endif

ifeq ($(use_ptm), y)
CFLAGS += -DUSE_PTM
endif

#===============================================================================
# OBJS for ECHO-B 
#-------------------------------------------------------------------------------
OBJS	+= onboot.o 

OBJS	+= ./$(ECHO_B_BASE_D)/lowlevel_init.o 
#OBJS	+= ./$(ECHO_B_BASE_D)/sdp_macros.o
#OBJS	+= ./$(ECHO_B_BASE_D)/sdp_init_basic.o
#OBJS	+= ./$(ECHO_B_BASE_D)/sdp_init_ddr_sp.o
OBJS	+= ./$(ECHO_B_BASE_D)/reset.o
#OBJS	+= ./$(ECHO_B_BASE_D)/v7_inv_dcache_all.o

OBJS	+= ./$(ECHO_B_BASE_D)/timer.o
OBJS	+= ./$(ECHO_B_BASE_D)/serial.o
OBJS	+= ./$(ECHO_B_BASE_D)/speed.o
OBJS	+= ./$(ECHO_B_BASE_D)/sp_setup.o
OBJS	+= ./$(ECHO_B_BASE_D)/echo_hci.o
OBJS	+= ./$(ECHO_B_BASE_D)/sdp_peri.o
OBJS	+= ./$(ECHO_B_BASE_D)/sdp_i2c.o
OBJS	+= ./$(ECHO_B_BASE_D)/usb_ohci.o
OBJS	+= ./$(ECHO_B_BASE_D)/usb.o
# for mmc boot
OBJS	+= ./$(ECHO_B_BASE_D)/init_sdp_mmc_core.o
OBJS	+= ./$(ECHO_B_BASE_D)/init_sdp_mmc.o

#ifeq ($(mmu),y)
#OBJS            += ./sp/mmu_sdp.o
#endif

OBJS	+= MMC_BM_NBL2_Common.o	
OBJS	+= cmdline.o	
OBJS	+= ./lib_generic/string.o
OBJS	+= ./lib_generic/vsprintf.o
OBJS	+= ./lib_generic/ctype.o
OBJS	+= ./lib_generic/utils.o
OBJS	+= ./lib_generic/download.o
OBJS	+= ./lib_generic/hexstr.o
##############################################################################
# Decide if revisioned board or not
#############################################################################
#CFLAGS = $(CPPFLAGS)
AFLAGS = $(CFLAGS)

GCCLIB_DIR	:= $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`)
GCCINC_DIR	:= $(shell $(CC) -print-file-name=include)

#===============================================================================
# Board define
#-------------------------------------------------------------------------------
#===============================================================================

.S.o:
	@echo "(AS) $@"
	@$(AS) $(AFLAGS) -c -o $@ $<

.c.o:
	@echo "(CC) $@"
	@$(CC) $(CFLAGS) $(INC_PATH) -c -o $@ $<

#===============================================================================
all : _print_version show_boundary $(TARGET_NAME).bin $(TARGET_NAME).txt $(TARGET_NAME).dis $(TARGET_NAME).ver

_print_version: $(TARGET_NAME)
	@echo "----------------------------------------"	
	@echo " Build information"
	@echo "----------------------------------------"	
	@echo "$(board_type), cpu=$(cpu) $(mem_map)MB $(smp_config) debug=$(dev) jp=$(country_jp) ptm=$(use_ptm)"
	@echo

$(TARGET_NAME).bin: $(TARGET_NAME)
	@$(OC) --gap-fill=0xff -O binary $(TARGET_NAME) $(TARGET_NAME).bin	

$(TARGET_NAME).txt: $(TARGET_NAME)
	@readelf -a  $(TARGET_NAME) > $@

$(TARGET_NAME).dis : $(TARGET_NAME)
	@$(OD) -d  $(TARGET_NAME) > $@

$(TARGET_NAME).ver :
	@date > $@

$(TARGET_NAME): $(OBJS) 
	@$(LD) -Bstatic -T onboot.lds -Ttext $(TEXT_BASE) -o $@ \
	$(OBJS) \
	--start-group -L$(GCCLIB_DIR) -lgcc --end-group -Map $@.map #--gc-sections



show_boundary: $(TARGET_NAME)


#===============================================================================
distclean: clean

clean:
	@echo delete $(TARGET_NAME)
	rm -f ./$(ECHO_B_BASE_D)/*.o
	rm -f *.o
	rm -f $(TARGET_NAME).dis $(TARGET_NAME).map $(TARGET_NAME).bin
	rm -f $(TARGET_NAME).txt $(TARGET_NAME).ver $(TARGET_NAME)
