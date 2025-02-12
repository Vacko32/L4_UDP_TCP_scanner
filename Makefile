CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++2a
SRC_DIR  = src
TARGET   = main

SRC      = $(SRC_DIR)/main.cpp $(SRC_DIR)/parsearg.cpp
OBJ      = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)
