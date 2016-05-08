CC		= cc -g
CFLAGS	= -I include/ -c
TARGET  = com
OBJS	= fifo.o

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

fifo.o: src/fifo.c
	$(CC) $(CFLAGS) $< -o $@
