
CC = gcc
CFLAG = -lpthread


all: single_variagle file_handle

single_variagle: local_variable_on_multi-thread.c
	gcc $^ $(CFLAG) -o $@ 

file_handle : file_handle_on_multi-thread.c
	gcc $^ $(CFLAG) -o $@ 
