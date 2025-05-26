CC     = gcc
CFLAGS = -Wall
LFLAGS =

PROG = escalona

SOURCE_DIRS = ./src/query ./src/graph ./src/conflict 
SOURCES = $(wildcard $(addsuffix /*.c, $(SOURCE_DIRS)))
OBJS = $(patsubst %.c,%.o,$(SOURCES))

.PHONY: debug clean purge

$(PROG): $(OBJS) src/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

vpath %.c $(SOURCE_DIRS)

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

src/main.o: src/main.c
	$(CC) -c $(CFLAGS) -o $@ $<

debug: CFLAGS += -g
debug: $(PROG)

clean:
	@rm -f *~ *.bak

purge: clean
	@rm -f *.o core a.out @rm -f src/main.o
	@rm -f $(OBJS)
	@rm -f $(PROG)