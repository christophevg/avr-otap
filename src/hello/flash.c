// via: http://www.nongnu.org/avr-libc/user-manual/group__avr__boot.html

#include <avr/boot.h>
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

void boot_program_page(uint32_t page, uint8_t *buf) {
  uint16_t i;
  uint8_t sreg;

  // disable interrupts.
  sreg = SREG;
  cli();
  eeprom_busy_wait();
  boot_page_erase(page);
  boot_spm_busy_wait(); // wait until the memory is erased.

  for(i=0; i<SPM_PAGESIZE; i+=2) {
    // set up little-endian word.
    uint16_t w = *buf++;
    w += (*buf++) << 8;
    boot_page_fill(page + i, w);
  }

  boot_page_write(page); // store buffer in flash page.
  boot_spm_busy_wait();  // wait until the memory is written.

  // reenable RWW-section again. we need this if we want to jump back
  // to the application after bootloading.
  boot_rww_enable ();

  // re-enable interrupts (if they were ever enabled).
  SREG = sreg;
}
