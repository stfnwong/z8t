# Z8T emulator makefile
# Stefan Wong 2018
#

# OUTPUT DIRS
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TEST_DIR=test
TEST_BIN_DIR=$(BIN_DIR)/test
TOOL_DIR=tools

# Tool options
CXX=g++
OPT=-O0
CXXFLAGS=-Wall -g2 -std=c++17 $(OPT)
TESTFLAGS=
#LDFLAGS =$(shell root-config --ldflags)
LDFLAGS=
LIBS= 
TEST_LIBS= 

# Object targets
INCS=-I$(SRC_DIR)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Unit tests 
TEST_SOURCES  = $(wildcard $(TEST_DIR)/*.cpp)
# Tools (program entry points)
TOOL_SOURCES = $(wildcard $(TOOL_DIR)/*.cpp)

.PHONY: clean

# Generic build 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

TEST_OBJECTS  := $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@ 

#TOOL_OBJECTS := $(TOOL_SOURCES:$(TOOL_DIR)/%.cpp=$(OBJ_DIR)/%.o)
#
#$(TOOL_OBJECTS): $(OBJ_DIR)/%.o : $(TOOL_DIR)/%.cpp
#	$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

# ==== TEST TARGETS ==== #
TESTS=test_lexer test_token test_cpu
$(TESTS): $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(OBJ_DIR)/$@.o\
		-o $(TEST_BIN_DIR)/$@ $(LIBS) $(TEST_LIBS)

## ===== TOOL TARGETS ===== # 
#TOOLS = c8asm c8dis
#
#$(TOOLS): $(OBJECTS) $(TOOL_OBJECTS)
#	$(CXX) $(LDFLAGS) $(OBJECTS) $(OBJ_DIR)/$@.o \
#		$(INCS) -o $(BIN_DIR)/$@ $(LIBS)

# Main targets 
all : test tools

test : $(TESTS)

tools : $(TOOLS)

clean:
	rm -rfv *.o $(OBJ_DIR)/*.o 
	rm -rfv bin/test/test_*

print-%:
	@echo $* = $($*)
