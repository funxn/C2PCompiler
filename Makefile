CC		= cc -g
CFLAGS	= -I include/
TARGET  = com
OBJS	= lib/ds.o src/scanner.o src/fifo.o src/stack.o src/yufa_func.o src/grammer.o
# OBJS = lib/ds.o src/yufa_func.o src/fifo.o

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

lib/ds.o: lib/ds.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

src/scanner.o: src/scanner.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

src/fifo.o: src/fifo.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

src/stack.o: src/stack.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

src/yufa_func.o: src/yufa_func.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@

src/grammer.o: src/grammer.c include/ds.h
	$(CC) $(CFLAGS) -c $< -o $@
