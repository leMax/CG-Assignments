#Makefile

# Print some information of this Makefile
QUITE=false
ifneq ($(QUITE),true)
$(info )
$(info Run `make` to build in the buildmode set in the Makefile (Release or Debug.))
$(info Run `make clean` to clean all *.o files and all build binaries)
$(info Run `make releaseclean` to clean all *.o files and the Release Binary)
$(info Run `make debugclean` to clean all *.o files and the Debug Binary)
$(info The output directory is either ../Release/ or ../Debug/ in relation to this Makefi1le.)
$(info )
endif

CXX = g++
CFLAGS = -Wall 
CFLAGS_DEBUG = -Wall -g3 -O1
INCLUDE = -I../include/ -I/usr/include/ -I/usr/local/include -I.
LIBPATH = -L/usr/lib/ -L/usr/local/lib -L../lib
LIBS = -lGL -lGLEW `sdl-config --cflags --libs` -lIL
BIN = oopframework

# Add your own files into the list and make sure that for every file in this list there is a corresponding *.h file
SRC = LogManager.cpp \
			RessourceManager.cpp \
			TextureManager.cpp \
			Texture.cpp \
			RenderEngine.cpp \
			InputManager.cpp \
      main.cpp \
	$(NULL)

OBJ=$(SRC:%.cpp=%.o)
HEADER=$(SRC:%.cpp=%.h)
# Change BUILD_MODE to Release will build in Release mode and generates performance optimized Code.
BUILD_MODE=Debug
#BUILD_MODE=Release


ifeq ($(BUILD_MODE),Release)
$(info Compiling in Release-Mode)
	CFLAGS += -g0 -O3
endif
ifeq ($(BUILD_MODE),Debug)
$(info Compiling in Debug-Mode)
	CFLAGS += -g3 -O1
endif
$(info )

default: prog

prog: $(OBJ)
	$(CXX) $(CFLAGS) -o $(BIN) $(OBJ) $(INCLUDEPATH) $(LIBPATH) $(LIBS)
	mkdir -p ../$(BUILD_MODE)/
	@echo "Moving binary to '../$(BUILD_MODE)/'"
	cp $(BIN) ../$(BUILD_MODE)/

%.o: %.cpp %.h
		$(CXX) $(CFLAGS) $(INCLUDE) -c $<

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf ../Release/$(BIN)
	rm -rf ../Debug/$(BIN)

debugclean:
	rm -rf $(OBJ) $(BIN)
	rm -rf ../Debug/$(BIN)
releaseclean:
	rm -rf $(OBJ) $(BIN)
	rm -rf ../Release/$(BIN)


