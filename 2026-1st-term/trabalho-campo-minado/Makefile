CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

SRCS = tabuleiro.c solver.c main.c
OBJS = $(SRCS:.c=.o)
TARGET = campo_minado

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c tabuleiro.h solver.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
