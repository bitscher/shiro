CXX := g++

CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix build/, $(CPP_FILES:.cpp=.o))

LDLIBS := $(shell sdl2-config --libs)
CXXFLAGS := \
	-Wall -Wextra -pedantic -Wold-style-cast -std=c++17 -g -Werror -O3 \
	$(shell sdl2-config --cflags)

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
