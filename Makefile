CXX=c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp src/DiskScheduler.cpp src/FileSystem.cpp
BIN = bin/os_project


all: build

build: $(BIN)

$(BIN): $(SRC)
	mkdir -p bin



	$(CXX) $(CXXFLAGS) -O2  -o $(BIN) $(SRC)

clean:
	rm -f $(BIN)

install: build
	mkdir -p dist
	cp $(BIN) dist/
.PHONY: all build debug clean install

