OPENGL_PATH ?= ../src/GLES_CM

CXXFLAGS += -std=c++17 -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../include

LDFLAGS += -L../src/GLES_CM -lm `sdl2-config --cflags --libs` -lGLES_CM

# Debugigng
ifdef DEBUG
	CXXFLAGS += -g -O0
else    
	CXXFLAGS += -O2 -DNDEBUG
endif

SRCS = main.cpp renderer.cpp md2.cpp utils.cpp

PROEJCT = md2viewer

HOSTNAME = $(shell cat /etc/resolv.conf | grep nameserver | awk '{print $$2}')

all: build
		
build: 
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(PROEJCT)
	
run:
	DISPLAY=$(HOSTNAME):0 LD_LIBRARY_PATH=$(OPENGL_PATH):$(LD_LIBRARY_PATH) ./$(PROEJCT) models/tekkaman.md2

clean:
	-rm -f $(PROEJCT)er *.o