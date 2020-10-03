CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
TARGETS=hw0101 hw0102 hw0103 hw0105



all: $(TARGETS)

.SECONDEXPANSION:
$(TARGETS): %: $$*.o

%.o: $@.c

clean:
	rm -rf $(TARGETS) $(addsuffix .o,$(TARGETS))
