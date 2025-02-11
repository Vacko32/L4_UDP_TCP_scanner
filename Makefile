CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++20
SRC_DIR  = src
TARGET   = main
SRC      = $(SRC_DIR)/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
