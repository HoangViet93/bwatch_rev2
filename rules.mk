%.bin: %.elf
	$(OBJCOPY) -O binary $(BINARY_PATH)$< $(BINARY_PATH)$@ 

%.hex: %.elf
	$(OBJCOPY) -O ihex $(BINARY_PATH)$< $(BINARY_PATH)$@ 

%.elf: $(OBJS_PATH) $(LDSCRIPT)
	$(LD) $(OBJS_PATH) $(LDFLAGS) -T$(LDSCRIPT) -o $(BINARY_PATH)$@

$(BINARY_PATH)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $< -o $@
