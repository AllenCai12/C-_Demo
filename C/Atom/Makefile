#CC=gcc
TOOL_CHAIN := arm-hisiv400-linux-
TARGET_DIR := /home/alan/work/tmp/nfs
CC = $(TOOL_CHAIN)gcc
SRC := $(wildcard *.c)
OBJS := ${SRC:.c=.o}
TARGET := $(TARGET_DIR)/main
FLAG := -ggdb

all: $(TARGET)
	@echo $(SRC)
$(TARGET): $(OBJS) 
	$(CC)  $(FALG) $^  -o $@
%.o : %.c
	$(CC) $(FLAG) -c $^ -o $@
clean :
	rm *.o
