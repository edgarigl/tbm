all: $(TARGET).hex

$(TARGET).hex: $(TARGET).bin
	@echo Generate $(@)
	hexdump -ve '/4 "%08X\n"' <$(^) >$(@)
