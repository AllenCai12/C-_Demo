CC = gcc

ifeq ($(DEBUG),true)
	debug := DEBUG
else
	debug := NODEBUG
endif

test: log_test.o
	gcc $< -o $@
log_test.o: log_test.c
	gcc -c $<  -o $@ -D$(debug)
clean:
	rm *.o test