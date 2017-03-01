# libopencm3 config
LIBDEPS += -lopencm3_stm32f1
LIB_OPENCM3_DIR = $(BASE_DIR)/lib/libopencm3
INCLUDE_PATH += -I$(BASE_DIR)/lib/libopencm3/include
LDFLAGS += -L$(LIB_OPENCM3_DIR)/lib
CFLAGS += -DSTM32F1
LIB_OPENCM3_NAME += $(LIB_OPENCM3_DIR)/lib/libopencm3_stm32f1.a

# free-rtos config
LIBDEPS += -lfree_rtos
LIB_FREERTOS_DIR = $(BASE_DIR)/lib/free_rtos
INCLUDE_PATH += -I$(LIB_FREERTOS_DIR)/include
LDFLAGS += -L$(LIB_FREERTOS_DIR)/lib
LIB_OPENCM3_NAME += $(LIB_FREERTOS_DIR)/lib/libfree_rtos.a
