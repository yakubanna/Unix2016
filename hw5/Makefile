TARGET = guess-server
CFLAGS = -Wall -Wextra --std=gnu99 -g -O2 -MD -MP
SRCS = io.c numbers.c server.c

all: $(TARGET)

$(TARGET): $(SRCS:%.c=%.o)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(SRCS:%.c=%.o) $(SRCS:%.c=%.d)

-include $(SRCS:%.c=%.d)
