BASE_DIR = ../..

BINARY = main

CONFIG += CONFIG_LED_B2
CONFIG += CONFIG_UART_PRINTF
CONFIG += CONFIG_DELAYMS

LIBDEPS += -lm

LDFLAGS += -u _printf_float

include $(BASE_DIR)/Makefile.common
