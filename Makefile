CC := g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix build/, $(CPP_FILES:.cpp=.o))
LD_FLAGS := -lSDL2 -lpthread
CC_FLAGS := -Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -Werror -O3

OBJ_DEBUG_FILES := $(addprefix build/, $(CPP_FILES:.cpp=_debug.o))
CC_DEBUG_FLAGS := -Wall -Wextra -Wold-style-cast -std=c++11 -g -O3 -D_DEBUG

all: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o build/shiro $^

debug: $(OBJ_DEBUG_FILES)
	$(CC) $(LD_FLAGS) -o build/shiro_debug $^

build/%_debug.o: %.cpp dir
	$(CC) $(CC_DEBUG_FLAGS) -c -o $@ $<

build/%.o: %.cpp dir
	$(CC) $(CC_FLAGS) -c -o $@ $<

dir:
	mkdir -p build/

clean:
	rm -rf build/