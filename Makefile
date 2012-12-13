## -*- Makefile -*-
##
## User: jasper
## Time: 22-jan-2012 16:41:42
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = g++
CXX = g++
BASICOPTS = -g -fpermissive
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_Herder=.


all: Herder

## Target: Herder
CPPFLAGS_Herder = \
	-Isrc/JEngine.cpp \
	-Isrc/JlibSDL.cpp \
	-Isrc/JlibHerder.cpp \
	-Isrc/TileManager.cpp \
	-Isrc/JRoute.cpp \
	-Isrc/HerderPState.cpp \
	-Isrc/HerderMState.cpp \
	-Isrc/HerderFState.cpp 

OBJS_Herder =  \
	$(TARGETDIR_Herder)/main.o \
	$(TARGETDIR_Herder)/TileManager.o \
	$(TARGETDIR_Herder)/JEngine.o \
	$(TARGETDIR_Herder)/JRoute.o \
	$(TARGETDIR_Herder)/HerderPState.o \
	$(TARGETDIR_Herder)/HerderMState.o \
	$(TARGETDIR_Herder)/HerderFState.o \
	$(TARGETDIR_Herder)/JlibSDL.o \
	$(TARGETDIR_Herder)/JlibHerder.o
SYSLIBS_Herder = -ldl 
USERLIBS_Herder = -Wl -Bstatic -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL `sdl-config --libs` $(SYSLIBS_Herder) 
DEPLIBS_Herder =      
LDLIBS_Herder = $(USERLIBS_Herder)


# Link or archive
$(TARGETDIR_Herder)/Herder: $(TARGETDIR_Herder) $(OBJS_Herder) $(DEPLIBS_Herder)
	$(LINK.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ $(OBJS_Herder) $(LDLIBS_Herder)


# Compile source files into .o files
$(TARGETDIR_Herder)/main.o: $(TARGETDIR_Herder) main.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ main.cpp

$(TARGETDIR_Herder)/TileManager.o: $(TARGETDIR_Herder) src/TileManager.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/TileManager.cpp

$(TARGETDIR_Herder)/JEngine.o: $(TARGETDIR_Herder) src/JEngine.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/JEngine.cpp

$(TARGETDIR_Herder)/JRoute.o: $(TARGETDIR_Herder) src/JRoute.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/JRoute.cpp

$(TARGETDIR_Herder)/HerderPState.o: $(TARGETDIR_Herder) src/HerderPState.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/HerderPState.cpp

$(TARGETDIR_Herder)/HerderMState.o: $(TARGETDIR_Herder) src/HerderMState.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/HerderMState.cpp

$(TARGETDIR_Herder)/HerderFState.o: $(TARGETDIR_Herder) src/HerderFState.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/HerderFState.cpp

$(TARGETDIR_Herder)/JlibSDL.o: $(TARGETDIR_Herder) src/JlibSDL.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/JlibSDL.cpp

$(TARGETDIR_Herder)/JlibHerder.o: $(TARGETDIR_Herder) src/JlibHerder.cpp
	$(COMPILE.cc) $(CCFLAGS_Herder) $(CPPFLAGS_Herder) -o $@ src/JlibHerder.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_Herder)/Herder \
		$(TARGETDIR_Herder)/main.o \
		$(TARGETDIR_Herder)/TileManager.o \
		$(TARGETDIR_Herder)/JEngine.o \
		$(TARGETDIR_Herder)/JRoute.o \
		$(TARGETDIR_Herder)/HerderPState.o \
		$(TARGETDIR_Herder)/HerderMState.o \
		$(TARGETDIR_Herder)/HerderFState.o \
		$(TARGETDIR_Herder)/JlibSDL.o \
		$(TARGETDIR_Herder)/JlibHerder.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-i386-Linux

