# Variables
#-------------------------------------------------------------------------------
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

ARCH_FLAG += -mthumb -mcpu=cortex-m3 -msoft-float -mfix-cortex-m3-ldrd

CFLAGS += -c -fno-common -Os -std=c99 -g $(ARCH_FLAG) -MD\
		  -Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls \
		  -Wstrict-prototypes -ffunction-sections -fdata-sections -Wall -Wundef -w

INCLUDE_PATH += -Iinclude
INCLUDE_PATH += -I../../include

SOURCE = $(wildcard *.c)
OBJS = $(SOURCE:.c=.o)

BINARY_PATH = ./lib/
VPATH += lib include
OBJS_PATH := $(addprefix $(BINARY_PATH), $(OBJS))

	
# Target
#-------------------------------------------------------------------------------
all: lib_dir libfree_rtos.a

lib_dir:
	mkdir -p lib

libfree_rtos.a: $(OBJS_PATH)  
	$(AR) rcs $(BINARY_PATH)$@ $(OBJS_PATH)

$(BINARY_PATH)%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $< -o $@

clean:
	rm -rf lib *.d

-include $(OBJS_PATH:.o=.d)
