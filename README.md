Retargeting Newlib-nano
------------------------

This is a simple test case of retargeting newlib nano which doesn't seem
to work. 

The goal here was to build a program using both the ARM
[embedded GCC](https://launchpad.net/gcc-arm-embedded) project and the
[libopencm3](https://github.com/libopencm3/libopencm3) project together. My 
test hardware is the STM32F4-Butterfly board which has an STM32F407G on it
and connects to the host system with ST-LINK V2. I'm using
[OpenOCD](http://openocd.sourceforge.net/) to load the software into the
Butterfly. All running on a Ubuntu 12.04LTS system with both libopencm3 and
the GCC Embedded toolchain installed.

The source for the example comes from the GCC Embedded samples directory. It
is called 'retarget' in that directory. I've copied over `gcc.ld` and modified
it to have FLASH and RAM at 0x08000000, and 0x20000000 respectively. I've
copied over makefile.conf and modified *it* to select for the Cortex M4 and
identify where the sources are (it picks up the startup code from there)

Diffs of my makefile.conf and the original:

<pre>
st-retarget$ diff makefile.conf ~/gcc-arm-none-eabi-4_8-2014q1/share/gcc-arm-none-eabi/samples/src/makefile.conf 
2c2
< CORTEX_M=4
---
> CORTEX_M=0
12c12
< BASE=/home/cmcmanis/gcc-arm-none-eabi-4_8-2014q1/share/gcc-arm-none-eabi/samples
---
> BASE=../..
</pre>

The Makefile was altered in some important ways;

* Pull in the modified makefile.conf from the current directory.
* Compile source with -mfloat-abi=hard -mfpu=fpv4-sp-d16
* Compile the source with -g so that I could see where it was stuck in gdb
* Link in libopencm3_stm32f4
* Identify the include file path for the libopencm3 include files.

Diffs of my Makefile and the original:

<pre>
st-retarget$ diff Makefile ~/gcc-arm-none-eabi-4_8-2014q1/share/gcc-arm-none-eabi/samples/src/retarget/Makefile                   
1c1                                                                                                 
< include makefile.conf                                                                             
---                                                                                                 
> include ../makefile.conf                                                                          
9,11d8                                                                                              
< CFLAGS+= -I../../libopencm3/include -DSTM32F4                                                     
< CFLAGS+= -mfloat-abi=hard -mfpu=fpv4-sp-d16                                                       
< CFLAGS+= -g                                                                                       
13c10                                                                                               
< LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld -L../../libopencm3/lib -lopencm3_stm32f4              
---                                                                                                 
> LDSCRIPTS=-L. -L$(BASE)/ldscripts -T gcc.ld
</pre>

If you look at retarget.c you will see that in `retarget_init()` I call my modified `_write` 
function to verify that its working as expected.

## What happens?

When I compile this code, it compiles and links with no errors. When I
run it the serial line prints "UART OK\n" and then it hangs. If I break
into gdb it is hung on the printf() line. Presumably it is hung *inside*
printf but the library isn't compiled -g so I can't tell.
