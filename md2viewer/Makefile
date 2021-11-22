OPENGL_PATH=../src/GLES_CM
#OPENGL_PATH=/home/blaise/dev/PowerVR_SDK/lib/Linux_x86_64

CXXFLAGS += -std=c++17 -O3 -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include
#CXXFLAGS += -std=c++17 -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -L../src/GLES_CM -I../include
LDFLAGS += -lm `sdl2-config --cflags --libs` -lGLES_CM

SRCS = main.cpp renderer.cpp md2.cpp utils.cpp

PROEJCT = md2viewer

all: build
		
build: 
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(PROEJCT)
	
run:
	LD_LIBRARY_PATH=$(OPENGL_PATH):$(LD_LIBRARY_PATH) ./$(PROEJCT) models/tekkaman.md2

clean:
	-rm -f $(PROEJCT)er *.o