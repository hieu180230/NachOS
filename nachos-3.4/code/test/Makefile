# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)


all: halt shell matmult sort testint testfloat testchar teststring taofile echo help ascii quicksort mergesort a b pingpong

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult
	
taofile.o: taofile.c
	$(CC) $(CFLAGS) -c taofile.c
taofile: taofile.o start.o
	$(LD) $(LDFLAGS) start.o taofile.o -o taofile.coff
	../bin/coff2noff taofile.coff taofile
	
echo.o: echo.c
	$(CC) $(CFLAGS) -c echo.c
echo: echo.o start.o
	$(LD) $(LDFLAGS) start.o echo.o -o echo.coff
	../bin/coff2noff echo.coff echo
	
help.o: help.c
	$(CC) $(CFLAGS) -c help.c
help: help.o start.o
	$(LD) $(LDFLAGS) start.o help.o -o help.coff
	../bin/coff2noff help.coff help
	
ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii
	
quicksort.o: quicksort.c
	$(CC) $(CFLAGS) -c quicksort.c
quicksort: quicksort.o start.o
	$(LD) $(LDFLAGS) start.o quicksort.o -o quicksort.coff
	../bin/coff2noff quicksort.coff quicksort

testint.o: testint.c
	$(CC) $(CFLAGS) -c testint.c
testint: testint.o start.o
	$(LD) $(LDFLAGS) start.o testint.o -o testint.coff
	../bin/coff2noff testint.coff testint

testfloat.o: testfloat.c
	$(CC) $(CFLAGS) -c testfloat.c
testfloat: testfloat.o start.o
	$(LD) $(LDFLAGS) start.o testfloat.o -o testfloat.coff
	../bin/coff2noff testfloat.coff testfloat

testchar.o: testchar.c
	$(CC) $(CFLAGS) -c testchar.c
testchar: testchar.o start.o
	$(LD) $(LDFLAGS) start.o testchar.o -o testchar.coff
	../bin/coff2noff testchar.coff testchar

teststring.o: teststring.c
	$(CC) $(CFLAGS) -c teststring.c
teststring: teststring.o start.o
	$(LD) $(LDFLAGS) start.o teststring.o -o teststring.coff
	../bin/coff2noff teststring.coff teststring

mergesort.o: mergesort.c
	$(CC) $(CFLAGS) -c mergesort.c
mergesort: mergesort.o start.o
	$(LD) $(LDFLAGS) start.o mergesort.o -o mergesort.coff
	../bin/coff2noff mergesort.coff mergesort
	
a.o: a.c
	$(CC) $(CFLAGS) -c a.c
a: a.o start.o
	$(LD) $(LDFLAGS) start.o a.o -o a.coff
	../bin/coff2noff a.coff a
	
b.o: b.c
	$(CC) $(CFLAGS) -c b.c
b: b.o start.o
	$(LD) $(LDFLAGS) start.o b.o -o b.coff
	../bin/coff2noff b.coff b
	
pingpong.o: pingpong.c
	$(CC) $(CFLAGS) -c pingpong.c
pingpong: pingpong.o start.o
	$(LD) $(LDFLAGS) start.o pingpong.o -o pingpong.coff
	../bin/coff2noff pingpong.coff pingpong
