CXX := g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix build/, $(CPP_FILES:.cpp=.o))

LDLIBS := -lSDL2 -lpthread
CXXFLAGS := -Wall -Wextra -pedantic -Wold-style-cast -std=c++11 -g -Werror -O3

OUTPUT_OPTION=-MMD -MP -o $@

all: build/shiro

build/shiro: build/ $(OBJ_FILES)
	$(CXX) $(LDLIBS) -o build/shiro $(OBJ_FILES)

build/%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

build/:
	mkdir -p build/

clean:
	rm -rf build/

-include $(wildcard build/*.d)
