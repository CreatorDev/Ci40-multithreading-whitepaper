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

LED_APP = led

SERIAL_APP = serial_read

PTHREAD_APP = pthread_read

all: led serial_read pthread_read

led:
	$(GCC) $(LED_APP).c $(CFLAGS) -I $(INCLUDE_DIR) -o $(LED_APP) -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS)  

serial_read:
	$(GCC) $(SERIAL_APP).c $(CFLAGS) -I $(INCLUDE_DIR) -o $(SERIAL_APP) -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS) 
 
pthread_read:
	$(GCC) $(PTHREAD_APP).c $(CFLAGS) -I $(INCLUDE_DIR) -o $(PTHREAD_APP) -L$(LD_DIR) $(CLICK_LINK) -L$(LD_DIR) $(CORE_LINK) $(LDFLAGS) 

clean:
	rm $(LED_APP) $(SERIAL_APP) $(PTHREAD_APP)
