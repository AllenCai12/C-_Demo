CC := gcc
TARGET := write read

all : $(TARGET)

write: write.c
	$(CC) $^ -o $@
read: read.c
	$(CC) $^ -o $@
clean:
	rm $(TARGET) -fr
