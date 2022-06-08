CC = gcc
CFLAGS = -g -Wall -lpthread
OBJS = gpio.o server.o
TARGET = server

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)
	
clean:
	rm -f *.o
	rm -f $(TARGET)



gpio.o: gpio.h gpio.c
server.o: server.c gpio.h