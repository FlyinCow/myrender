LIBS = -lm
OBJS = $(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))
CPPS = $(wildcard src/*.cpp)
CXXFLAGS = -Wall -Og -ggdb -g3 -I./include
LDFLAGS =

TARGET = main
.PHONY:all
all:$(TARGET)

$(TARGET):$(OBJS)
	g++ $(OBJS) $(LDFLAGS) -o $(TARGET)

$(OBJS):build/%.o:src/%.cpp
	g++ -c $< $(CXXFLAGS) -o $@

.PHONY:show
show:
	HoneyView.exe test.bmp

.PHONY:clean
clean:
	rm -rf build/*
	rm -f main
	rm -f *.bmp