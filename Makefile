# https://stackoverflow.com/questions/2908057/can-i-compile-all-cpp-files-in-src-to-os-in-obj-then-link-to-binary-in
SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
# LDFLAGS := ...
# CPPFLAGS := ...
CXXFLAGS := -std=c++14

test: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

CXXFLAGS += -MMD
-include $(OBJ_FILES:.o=.d)

clean:
	rm -f test
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OBJ_DIR)/*.d

# g++ = clang
# g++-12 = g++
# NOTE: we can compile simply with g++ -std=c++14 -o test src/*.cpp