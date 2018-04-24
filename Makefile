#
# This program is a common source code compiler, deposit in the SRC folder C file, 
# INCLUDE deposit folder header files, the third party inventory in the LIB,
# can automatically compile the project.The OBJ files will be stored in the OBJ directory,
# and the binaries will be stored in the BIN directory,
# which will be created automatically when they are not present.
# 

OBJ = ./obj
BIN = ./bin
LIB = ./lib

DIRS := $(shell find ./include ./src -type d)
VPATH := $(DIRS)

C_NOTDIR_FILES = $(foreach dir, $(DIRS),$(notdir $(wildcard $(dir)/*.c)))
notdir_obj = $(patsubst %.c,%.o,  $(C_NOTDIR_FILES))
obj = $(foreach dir,$(notdir_obj),$(OBJ)/$(dir))
INC_DIR := $(shell find ./include -type d)
INC_DIRS = $(foreach dir,$(INC_DIR), -I$(dir))

TARGET = $(BIN)/main
CFLAGS = $(INC_DIRS)
#CC = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
CC = gcc
#LPTHREAD = -lpthread -ljpeg -L$(LIB)
LPTHREAD = -lpthread

$(TARGET):$(notdir_obj)
	@if [ ! -d $(BIN) ]; then \
		mkdir $(BIN) ; \
	fi
	@$(CC) $(obj) -o $@ $(LPTHREAD)
	@echo "start ld obj..."
	@echo "Compilete done."
$(notdir_obj):%.o:%.c
	@if [ ! -d $(OBJ) ]; then \
		mkdir $(OBJ) ; \
	fi
	@echo "Compiling $< ==> $@"
	@${CC} $(CFLAGS) -c $< -o $(OBJ)/$@;

.PHONY: test
test:
	@$(TARGET) #test build is OK?

.PHONY: clean
clean:
	@echo "..........Start clean..........."
	@rm -f $(OBJ)/*
	@echo rm -f *.obj
	@rm -f $(BIN)/*
	@echo rm -f main
	@echo ".........Compilete done........."

