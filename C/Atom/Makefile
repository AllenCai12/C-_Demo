CC=gcc

SRC := $(wildcard *.c)
OBJS := ${SRC:.c=.o}
TARGET := main

all: $(TARGET)
	@echo $(SRC)
$(TARGET): $(OBJS) 
	$(CC) $^ -o $@
%.o : %.c
	$(CC) -c $^ -o $@
