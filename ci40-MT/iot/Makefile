TOOLCHAIN_DIR=../mips-mti-linux-gnu/2016.05-03/

BIN_DIR=$(TOOLCHAIN_DIR)bin/

TOOL_PREFIX=mips-mti-linux-gnu-

GCC=$(BIN_DIR)$(TOOL_PREFIX)gcc

CFLAGS=-std=c99 -Wall -O3 -msoft-float -g -EL -DRTELINUX -D_GNU_SOURCE -march=interAptiv -DNDEBUG -pthread -static 

LD_DIR=../letmecreate/

INCLUDE_DIR=../letmecreate/include/

LDFLAGS=-lpthread -lrt -lm 

CORE_LINK=-l:libletmecreate_core.a

CLICK_LINK=-l:libletmecreate_click.a

all: led serial_read pthread_read

led:
	$(GCC) led.c $(CFLAGS) -I $(INCLUDE_DIR) -o led.exe -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS)  

serial_read:
	$(GCC) serial_sensor_read.c $(CFLAGS) -I $(INCLUDE_DIR) -o serial_read.exe -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS) 
 
pthread_read:
	$(GCC) pthread_sensor_read.c $(CFLAGS) -I $(INCLUDE_DIR) -o pthread_read.exe -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS) 

clean:
	rm *.exe
