#ifndef __FLASH_H
#define __FLASH_H

#include <stdint.h>
#include <avr/boot.h>

void boot_program_page(uint32_t page, uint8_t *buf) BOOTLOADER_SECTION;

#endif
