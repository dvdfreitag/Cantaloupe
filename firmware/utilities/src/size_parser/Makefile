SRC := size_parser.c

TARGET := ../../size_parser

CC := gcc
CFLAGS := -Wall -Wextra -g

TARGET: $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
	rm $(TARGET)