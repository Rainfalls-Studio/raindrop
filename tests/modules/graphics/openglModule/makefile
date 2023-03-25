# project variables
STD_VERSION = c++17
VERSION = 0.1.0
EXEC = openglRenderModule
CXX = g++
FLAGS = 
RAW_LIBS = Raindrop glad SDL2main SDL2 mingw32 dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 setupapi version uuid
RAW_DEFINES = RAINDROP_VERSION='"$(VERSION)"' RAINDROP_ASSERTS

# directories
BIN = ../../../out/modules
SRC = .
OBJ = .obj
LIB = -L ../../../libs -L ../../../../out/
INCLUDES = -I . -I ../../../../src/ -I ../

ifeq ($(OS), Windows_NT)
	RAW_LIBS += mingw32
endif

# treated variables
DBG_FLAGS = -g3
RELEASE_FLAGS = -Wall -O2 -DNDEBUG

LIBS = $(addprefix -l, $(RAW_LIBS))
DEFINES = $(addprefix -D, $(RAW_DEFINES))

ifeq ($(MAKECMDGOALS), dbg)
	FLAGS += $(DBG_FLAGS)
endif

ifeq ($(MAKECMDGOALS), dbgRebuild)
	FLAGS += $(DBG_FLAGS)
endif

ifeq ($(MAKECMDGOALS), release)
	FLAGS += $(RELEASE_FLAGS)
endif

# source files
SRCS = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/**/**/*.cpp) $(wildcard $(SRC)/**/**/**/*.cpp)
OBJS := $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SRCS)))

all: $(EXEC)

dbg: all
dbgRebuild: rebuild
release: rebuild

rebuild: clean $(EXEC)

clean:
	@del $(OBJ)\*.o

$(EXEC) : $(OBJS)
	g++ -shared $(OBJS) -o $(BIN)/$(EXEC).dll $(LIB) $(LIBS) -W

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) -shared -std=$(STD_VERSION) -o $@ -c $< $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

$(OBJ)/%.o : $(SRC)/*/%.cpp
	$(CXX) -shared -std=$(STD_VERSION) -o $@ -c $< $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

$(OBJ)/%.o : $(SRC)/*/*/%.cpp
	$(CXX) -shared -std=$(STD_VERSION) -o $@ -c $< $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)
	
$(OBJ)/%.o : $(SRC)/*/*/*/%.cpp
	$(CXX) -shared -std=$(STD_VERSION) -o $@ -c $< $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

help:
	@echo Targets:
	@echo   - all             : build the program
	@echo   - rebuild         : rebuild all the files of the program
	@echo   - release         : build the program in release mode (without any debug)
	@echo   - dbg             : build the program in debug mode
	@echo   - dbgRebuild      : rebuild all the files of the program in debug mode
	@echo   - clean           : remove the object and output files
	@echo --------------------------------------------------------------------------
	@echo Project variables:
	@echo   - version         : $(VERSION)
	@echo   - compiler        : $(CXX)
	@echo   - std version     : $(STD_VERSION)
	@echo   - flags           : $(FLAGS)
	@echo   - libs            : $(RAW_LIBS)
	@echo   - defines         : $(RAW_DEFINES)