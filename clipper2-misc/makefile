CXX = clang++
CXXFLAGS = -shared -std=c++17 -fPIC
INCLUDE_DIR = ../include
SRC_DIR = .
TARGET = libclipper2.dylib
SOURCES = $(SRC_DIR)/clipper.engine.cpp \
          $(SRC_DIR)/clipper.offset.cpp \
          $(SRC_DIR)/clipper.rectclip.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) -I$(INCLUDE_DIR)

clean:
	rm -f $(TARGET)
