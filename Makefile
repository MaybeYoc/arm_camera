DIRS := $(shell find . -type d)
VPATH := $(DIRS)
FILES = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
FILES1 = $(foreach dir, $(DIRS),$(notdir $(wildcard $(dir)/*.c)))
obj = $(patsubst %.c,%.o, $(notdir $(FILES)))
objd = $(foreach dir,$(obj),./obj/$(dir))
INC_DIR := $(shell find ./include -type d)
INC_DIRS = $(foreach dir,$(INC_DIR), -I$(dir))

CFLAGS = $(INC_DIRS)

#CC = /usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
CC = gcc
#LPTHREAD = -lpthread -ljpeg -L./lib
LPTHREAD = 


./bin/main:$(obj)
	@if [ ! -d "./bin" ]; then \
		mkdir ./bin ; \
	fi
	@$(CC) $(objd) -o $@ $(LPTHREAD)
	@echo "start ld obj..."
	@echo "Compilete done."

$(obj):%.o:%.c
	@if [ ! -d "./obj" ]; then \
		mkdir ./obj ; \
	fi
	@echo "Compiling $< ==> $@"
	@${CC} $(CFLAGS) -c $< -o ./obj/$@;


.PHONY: test
test:
	@./bin/main    #test build is OK?

.PHONY: clean
clean:
	@echo "..........Start clean..........."
	@rm -f ./obj/*
	@echo rm -f *.obj
	@rm -f ./bin/*
	@echo rm -f main
	@echo ".........Compilete done........."

