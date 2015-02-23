#include <stdint.h>
#include <avr/boot.h>

#include "flash.h"

int main(void) {
  uint8_t buffer[256];
  for(int i=0; i<256; i++) {
    buffer[i] = i;
  }
      
  boot_program_page(0x100, buffer);

  while(1) {} // embedded programs should never return ;-)
  return 0;
}
