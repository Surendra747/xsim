CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -D_CRT_SECURE_NO_WARNINGS
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2 -flto
LDFLAGS =
SRC_DIR = Source
BIN_DIR = Bin
OBJ_DIR = Obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

TARGET = $(BIN_DIR)/xsimulator

.PHONY: all debug release clean

all: debug

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

release: CXXFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
