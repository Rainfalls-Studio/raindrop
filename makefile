# project variables
STD_VERSION = c++17
VERSION = 0.1.0
EXEC = Raindrop.lib
CXX = g++
FLAGS = 
RAW_LIBS = 
RAW_DEFINES = RAINDROP_VERSION='"$(VERSION)"' RAINDROP_ASSERTS

# directories
BIN = out
SRC = src
OBJ = .obj
LIB = libs
INCLUDES = -I src/

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

ifeq ($(MAKECMDGOALS), dbgrebuild)
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
	ar crs $(BIN)/$(EXEC) $(OBJ)/*.o

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -L $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

$(OBJ)/%.o : $(SRC)/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -L $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

$(OBJ)/%.o : $(SRC)/*/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -L $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)
	
$(OBJ)/%.o : $(SRC)/*/*/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -L $(LIB) $(DEFINES) $(FLAGS) $(INCLUDES)

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