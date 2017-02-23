%.bin: %.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@ 

%.elf: $(OBJS) $(LDSCRIPT)
	$(LD) $(OBJS) $(LDFLAGS) -T$(LDSCRIPT) $(LIBDEPS) -o $@

%.o: %.c 
	$(CC) $(CFLAGS) $< -o $@