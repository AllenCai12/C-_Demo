CC=g++

INC := -I inc/

MK=mkdir
DIR_BIN := bin
DIR_OBJ := obj
DIR_SRC := src
DIRS := $(DIR_OBJ) $(DIR_BIN)

TARGET = $(DIR_BIN)/MAIN

SRC	:= ${wildcard src/*.cpp}

#将src/*.cpp 转为 obj/*.o
OBJS := ${SRC:.cpp=.o}
OBJS := ${notdir $(OBJS)}
OBJS := ${addprefix $(DIR_OBJ)/, $(OBJS)}

RM := rm -fr

all	: $(DIRS) $(TARGET) 

$(DIRS) :  
	$(MK) $@

$(TARGET) : $(OBJS) 
	$(CC) $^ -o $@
$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp 
	$(CC) $^ $(INC) -c -o $@
clean:
	$(RM) $(DIR_OBJ) $(TARGET)

test:
	@echo $(SRC)
	@echo $(OBJS)
	@echo $(TARGET)
