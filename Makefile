CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++2a -I$(SRC_DIR)
SRC_DIR  = src
TARGET   = main  
TESTDIR  = tests

SRC_MAIN = $(SRC_DIR)/main.cpp $(SRC_DIR)/parsearg.cpp $(SRC_DIR)/main.cpp
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)

SRC_TEST = $(TESTDIR)/test.cpp $(SRC_DIR)/parsearg.cpp $(SRC_DIR)/main.cpp
OBJ_TEST = $(SRC_TEST:.cpp=.o)

all: main test
main: $(OBJ_MAIN)
	$(CXX) $(CXXFLAGS) $(OBJ_MAIN) -o main
test: $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) $(OBJ_TEST) -o test
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f main test $(OBJ_MAIN) $(OBJ_TEST)