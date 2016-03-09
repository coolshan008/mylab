CC      = gcc
LD      = ld
CFLAGS  = -c -static -ggdb -m32 -MD -Wall -O2 -fno-builtin \
		  -fno-stack-protector -fno-omit-frame-pointer
LDFLAGS = -melf_i386

CFILES  = $(shell find  -name "*.c")
OBJS    = $(CFILES:.c=.o)

userblock:$(OBJS)
	$(LD) $(LDFLAGS) -e main -o user $(OBJS)
	@perl genboot.pl user

-include $(OBJS:.o=.d)

.PHONY: clean

#userblock: syscall.c usr.c lib.h const.h types.h
#	gcc -c -m32 -static -ggdb -MD -Wall -Werror -O1  \
#	    -fno-builtin -fno-stack-protector -fno-omit-frame-pointer\
#		  usr.c syscall.c 
#	ld -melf_i386 -e main  usr.o syscall.o -o user.o
#	@objcopy -S -O binary -j .text user.o user	
#	@perl genboot.pl user

clean:
	rm -rf user *.o *.d
