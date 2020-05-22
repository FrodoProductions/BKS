NASM=nasm
NASMFLAGS=-f elf64
CFLAGS=-std=c11 -Wall -Wextra -pedantic -O2
CPPFLAGS=-MMD -MF $*.d -D_POSIX_C_SOURCE=199309L


programs= evil_Scheller_Brandenburg kill_Scheller_Brandenburg
all: $(programs)

-include *.d

# pattern rules
%: %.o
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

%.o: %.asm
	$(NASM) -o $@ $(NASMFLAGS) $<
%.o: %.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $<

%.d: ;

clean:
	@$(RM) -fv $(programs) $(programs:=.o) $(programs:=.d)

.SUFFIXES:
.PHONY: all clean
.PRECIOUS: $(programs:=.o)
