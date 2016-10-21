TOOLCHAIN_DIR=../mips-mti-linux-gnu/2016.05-03/

BIN_DIR=$(TOOLCHAIN_DIR)bin/

TOOL_PREFIX=mips-mti-linux-gnu-

GCC=$(BIN_DIR)$(TOOL_PREFIX)gcc
APP = multi_thread_acc
CFLAGS=-std=c99 -Wall -O3  -g -EL -DRTELINUX -D_GNU_SOURCE -march=interAptiv -DNDEBUG -pthread -static

LDFLAGS=-lpthread -lrt -lm

all:
	$(GCC) $(APP).c $(CFLAGS) -o ./$(APP) $(LDFLAGS)
clean:
	rm $(APP)
