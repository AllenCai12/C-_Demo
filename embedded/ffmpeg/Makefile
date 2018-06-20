CC := gcc
TARGET := ffmpeg
FLAG :=  -lavformat -lavcodec -lavutil -lswscale 

$(TARGET): ffmpeg.c 
	$(CC) $< -o $@ $(FLAG)
