# Use newlib-nano. 
USE_NANO=--specs=nano.specs
USE_NOHOST=--specs=nosys.specs

BASE=/home/cmcmanis/gcc-arm-none-eabi-4_8-2014q1/share/gcc-arm-none-eabi/samples

# Compiler & Linker
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++

# Options for specific architecture
ARCH_FLAGS=-mthumb -mcpu=cortex-m4

# Startup code
STARTUP=$(BASE)/startup/startup_ARMCM4.S

# -Os -flto -ffunction-sections -fdata-sections to compile for code size
CFLAGS=$(ARCH_FLAGS) $(STARTUP_DEFS) -Os -flto -ffunction-sections -fdata-sections
CXXFLAGS=$(CFLAGS)

# Link for code size
GC=-Wl,--gc-sections

# Create map file
MAP=-Wl,-Map=$(NAME).map
NAME=retarget

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin
CFLAGS+= -I../../libopencm3/include -DSTM32F4
CFLAGS+= -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS+= -g

LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld -L../../libopencm3/lib -lopencm3_stm32f4

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

$(NAME).axf: main.c $(NAME).c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

clean:
	rm -f *.axf *.map *.o
