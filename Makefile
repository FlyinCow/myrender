LIBS = -lm
CPPS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,build/%.o,$(CPPS))
CXXFLAGS = -Wall -I./include -O0 -ggdb -g3
LDFLAGS =

TARGET = main
.PHONY:all
all:$(TARGET)

$(TARGET):$(OBJS)
	g++ $(OBJS) $(LDFLAGS) -o $(TARGET)

$(OBJS):build/%.o:src/%.cpp
	g++ -c $< $(CXXFLAGS) -o $@

.PHONY:run
run:$(TARGET)
	./$(TARGET)

.PHONY:show
show:run
	HoneyView.exe test.bmp

.PHONY:clean
clean:
	rm -rf build/*
	rm -f main
	rm -f *.bmp