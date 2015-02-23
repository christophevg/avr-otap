# each module needs a starting point/target
ifndef TARGET
	TARGET = main
endif

# MCU name (avrdude profile)
ifndef MCU
	MCU = atmega328p
endif

# processor frequency depends on project
ifndef F_CPU
	F_CPU = 18000000
endif

# additional sources to be compiled
ifndef MORE_SRC
	MORE_SRC =
endif

# changing the programming protocol
ifndef AVRDUDE_PROGRAMMER
	AVRDUDE_PROGRAMMER = jtag2isp
endif

# defining the include path
ifndef INCLUDE_PATH
	INCLUDE_PATH=.
endif

# output format. (can be srec, ihex, binary)
FORMAT = ihex

SRC = $(TARGET).c $(MORE_SRC)
OBJ = $(SRC:.c=.o)
LST = $(SRC:.c=.lst)

# optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# providing the CPU's frequency
CDEFS = -DF_CPU=$(F_CPU)UL
# providing the MCU
CDEFS += -DMCU=$(MCU)
#ifdef MORE_CDEFS
	CDEFS += $(MORE_CDEFS)
#endif

# compiler options
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = $(CDEFS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(CSTANDARD)

# assembler options
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

# library options
MATH_LIB = -lm

# linker options
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(MATH_LIB)
# This adds support for printf with float
LDFLAGS += -lm -lprintf_flt -Wl,-u,vfprintf -Wl,-Map=$(TARGET).map

# avrdude
AVRDUDE_PORT = usb:5a:cb
AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)

# define programs and commands.
SHELL = sh
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = avrdude
REMOVE = rm -f
COPY = cp

# combine all necessary flags and optional flags.
# add target processor to flags.
ALL_CFLAGS = -mmcu=$(MCU) -I$(INCLUDE_PATH) $(CFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I$(INCLUDE_PATH) -x assembler-with-cpp $(ASFLAGS)

# default target.
all: build sizeafter

build: elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
AVRMEM = avr-mem.sh $(TARGET).elf $(MCU)

sizeafter:
	@if test -f $(TARGET).elf; then echo; $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi

# program the device.  
program: $(TARGET).hex $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

clean:
	@rm -rf *.hex *.eep *.cof *.elf *.map *.sym *.lss *.lst *.o *.s

# create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo "--- creating HEX image"
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo "--- creating EEPROM"
	@-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
		 -O $(FORMAT) $< $@
# --change-section-lma .eeprom=0

# create extended listing file from ELF output file.
%.lss: %.elf
	@echo "--- creating extended listing file"
	@$(OBJDUMP) -h -S $< > $@

# create a symbol table from ELF output file.
%.sym: %.elf
	@echo "--- creating symbol table"
	@$(NM) -n $< > $@

# link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo "--- linking $@"
	@$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# compile: create object files from C source files.
%.o : %.c
	@echo "--- compiling $<"
	@$(CC) -c $(ALL_CFLAGS) $< -o $@

# compile: create assembler files from C source files.
%.s : %.c
	@echo "--- generating assembler for $<"
	@$(CC) -S $(ALL_CFLAGS) $< -o $@

# assemble: create object files from assembler source files.
%.o : %.S
	@echo "--- assembling $<"
	@$(CC) -c $(ALL_ASFLAGS) $< -o $@

.PHONY : all build elf hex eep lss sym sizeafter program clean
