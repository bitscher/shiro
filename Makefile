CC := g++
CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix build/, $(CPP_FILES:.cpp=.o))
LD_FLAGS := -lSDL2 -lpthread
CC_FLAGS := -Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -Werror -O3

all: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o build/shiro $^

build/%.o: %.cpp dir
	$(CC) $(CC_FLAGS) -c -o $@ $<

dir:
	mkdir -p build/

clean:
	rm -rf build/