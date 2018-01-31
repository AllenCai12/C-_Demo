CC=g++

SRC	:= ${wildcard *.cpp}
OBJS := ${SRC:.cpp=.o}
RM := rm

MAIN : $(OBJS) 
	$(CC) $^ -o $@
./%.o : %.cpp 
	$(CC) $^ -c -o $@
clean:
	$(RM) $(OBJS) MAIN	


