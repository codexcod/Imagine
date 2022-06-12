.PHONY: all clean

BIN = main loader

CPP = main.cpp filters.cpp ppm.cpp
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: main

clean:
	rm -f $(BIN) $(OBJ)

loader:
	g++ -pthread -o loader loader.cpp filters.cpp ppm.cpp

main: main.o filters.o ppm.o
	g++ -pthread -o $@ $^

%.o: %.cpp
	g++ -pthread -c -o $@ $<
