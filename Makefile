TARGET = compare
CFLAGS = -Wall -g

all:$(TARGET)

compare:compare.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TARGET)
