BOOTLOAD = 0x1E000
LDFLAGS += -Wl,--section-start=.bootloader=${BOOTLOAD}

MCU=atmega1284p
MORE_SRC=flash.c

include ../../lib/Makefile
