include makefile.conf
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

$(NAME)-$(CORE).axf: main.c $(NAME).c $(STARTUP)
	$(CC) $^ $(CFLAGS) $(LFLAGS) -o $@

clean:
	rm -f *.axf *.map *.o
