CC := g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix build/, $(CPP_FILES:.cpp=.o))
LD_FLAGS := -lSDL2 -lpthread
CC_FLAGS := -Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -Werror -O3

OBJ_DEBUG_FILES := $(addprefix build/, $(CPP_FILES:.cpp=_debug.o))
CC_DEBUG_FLAGS := -Wall -Wextra -Wold-style-cast -std=c++11 -g -O3 -D_DEBUG

all: build/shiro

debug: build/shiro_debug

build/shiro: build/ $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o build/shiro $(OBJ_FILES)

shiro_debug: build/ $(OBJ_DEBUG_FILES)
	$(CC) $(LD_FLAGS) -o build/shiro_debug  $(OBJ_DEBUG_FILES)

build/%_debug.o: %.cpp
	$(CC) $(CC_DEBUG_FLAGS) -c -o $@ $<

build/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

build/:
	mkdir -p build/

clean:
	rm -rf build/
