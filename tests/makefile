SRC = srcs
LIB = libs
OUT = out

LIBS = -lgmock_main -lgmock -lgtest_main -lgtest -lRaindrop
SRCS = $(wildcard $(SRC)/*.cpp)
OUTS = $(patsubst %.cpp, $(OUT)/%.exe, $(notdir $(SRCS)))

all: $(OUTS)

clear: del $(OUT)/*.exe

rebuild: clear all

$(OUT)/%.exe: $(SRC)/%.cpp
	$(CXX) -std=c++17 -o $@ $< -I . -I ../src/ $(LIBS) -L$(LIB) -L../out/