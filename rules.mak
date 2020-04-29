ARFLAGS		?= r
RANLIB		?= @:

startup.o:	$(OS)/runtime/$(ARCH)/$(STARTUP)
		$(CC) $(ASFLAGS) -c $< -o $@\

libuos.a:	always
		@for m in $(MODULES); do\
			[ -d $(OBJ)/$$m ] || mkdir $(OBJ)/$$m;\
			$(MAKE) -f$(OS)/module.mak -C$(OBJ)/$$m\
				OS=$(OS) TARGET=$(TARGET) DRIVERS=$(DRIVERS) MODULE=$$m;\
		done
		$(RANLIB) libuos.a

always:

.SUFFIXES:	.i .srec .hex .dis .cpp .cxx .fl .bin .elf

.PHONY:		$(MODULES) depend

.c.o:
		@[ -d .deps ] || mkdir .deps
		$(CC) $(CFLAGS) $(DEPFLAGS) -c $<

.cpp.o:
		@[ -d .deps ] || mkdir .deps
		$(CXX) $(CFLAGS) $(DEPFLAGS) -c $<

.cxx.o:
		@[ -d .deps ] || mkdir .deps
		$(CXX) $(CFLAGS) $(DEPFLAGS) -c $<

.S.o:
		@[ -d .deps ] || mkdir .deps
		$(CC) $(ASFLAGS) $(DEPFLAGS) -c $<

.c.s:
		$(CC) $(CFLAGS) -S $<

.c.i:
		$(CC) $(CFLAGS) -E $< > $@

.c.elf:
		@[ -d .deps ] || mkdir .deps
		$(CC) $(LDFLAGS) $(CFLAGS) $(DEPFLAGS) $< $(LIBS) -o $@

.o.elf:
		$(CC) $(LDFLAGS) $< $(LIBS) -o $@
		cp *.elf $(BIN)

.fl.cxx .fl.h:
		fluid -c $<

.elf.srec:
		$(OBJCOPY) -O srec $< $@
		@chmod -x $@

.elf.hex:
		$(OBJCOPY) -O ihex $< $@
		@chmod -x $@
		cp *.hex $(BIN)
		rm -rf *~ $(OUT)

.elf.bin:
		$(OBJCOPY) -O binary $< $@
		@chmod -x $@
		cp *.bin $(BIN)
		rm -rf *~ $(OUT)

.elf.dis:
		$(OBJDUMP) -d -z -S $< > $@

ifeq (.deps, $(wildcard .deps))
-include .deps/*.dep
endif
