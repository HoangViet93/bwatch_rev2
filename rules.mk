%.bin: %.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@ 

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@ 

%.elf: $(OBJS) $(LDSCRIPT)
	$(LD) $(OBJS) $(LDFLAGS) -T$(LDSCRIPT) $(LIBDEPS) -o $@

%.o: %.c 
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $< -o $@