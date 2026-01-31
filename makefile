CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc/headers

TARGET = main

SRC = main.cpp $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
