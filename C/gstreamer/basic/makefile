CC := gcc
CFLAG := `pkg-config --cflags --libs gstreamer-1.0`
TARGET := basic
SRC	:= ${wildcard *.c}

$(TARGET): $(SRC)
	$(CC) $< -o $@ $(CFLAG) 
