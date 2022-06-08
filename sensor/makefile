CC = gcc
CFLAGS = -lpthread 
OBJS = gpio.o hs_client.o
TARGET = hs_client

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)
	
clean:
	rm -f *.o
	rm -f $(TARGET)



gpio.o: gpio.h gpio.c
hs_client.o: hs_client.c gpio.h