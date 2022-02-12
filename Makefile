#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# wildcard function recursive
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
CXXFLAGS	:= -std=c++17 -Wall -Wextra -g

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= build

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

# define shader directory
SHADER	:= shader

ASSETS  := assets

ifeq ($(OS),Windows_NT)
MAIN		:= Game.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH 	= $(subst /,\,$1)
RM			:= del /q /f
MD			:= mkdir
COPY 		:= xcopy "$(SHADER)" "$(OUTPUT)\$(SHADER)"/S/D/I/Y && xcopy "$(ASSETS)" "$(OUTPUT)\$(ASSETS)"/S/D/I/Y && xcopy "$(LIB)\glew32.dll" "$(OUTPUT)" && xcopy "$(LIB)\glfw3.dll" "$(OUTPUT)"
else
MAIN		:= Game
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH 	= $1
RM 			:= rm -f
MD			:= mkdir -p
COPY		:= mkdir -p $(SHADER) $(OUTPUT)/$(SHADER) && cp $(SHADER) $(OUTPUT)/$(SHADER) && mkdir -p $(ASSETS) $(OUTPUT)/$(ASSETS) && cp $(ASSETS) $(OUTPUT)/$(ASSETS)
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%)) -lglfw3dll -lglfw3 -lopengl32 -lglew32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

# define the C source files
SOURCES		:= $(call rwildcard,$(SOURCEDIRS),*.cpp)

# define the C object files 
OBJECTS		:= $(SOURCES:.cpp=.o)

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	$(COPY)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!