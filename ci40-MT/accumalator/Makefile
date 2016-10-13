TOOLCHAIN_DIR=../mips-2016.05/

BIN_DIR=$(TOOLCHAIN_DIR)bin/

TOOL_PREFIX=mips-linux-gnu-

GCC=$(BIN_DIR)$(TOOL_PREFIX)gcc

CFLAGS=-std=c99 -Wall -O3  -g -EL -DRTELINUX -D_GNU_SOURCE -march=1004kc -DNDEBUG -pthread -static

LDFLAGS=-lpthread -lrt -lm

all:
	$(GCC) multi_thread_acc.c $(CFLAGS) -o ./multi_thread_acc.exe $(LDFLAGS)
clean:
	rm *.exe
