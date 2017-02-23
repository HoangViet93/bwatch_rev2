#!/bin/sh

# Let take two parameter 
OPENOCD_CFG=$1
BIN_IMAGE=$2

# send SIGKILL signal to kill old openocd process
killall -s 9 openocd
# openocd flash command line
openocd -f ${OPENOCD_CFG} -c init -c "reset halt" -c "flash write_image erase ${BIN_IMAGE}" \
		-c "verify_image ${BIN_IMAGE}" -c reset -c shutdown 