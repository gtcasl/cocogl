OPENGL_PATH ?= ../bin

CXXFLAGS += -std=c++17 -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../include

LDFLAGS += -L$(OPENGL_PATH) -lm `sdl2-config --cflags --libs` -lGLES_CM

# Debugigng
ifdef DEBUG
	CXXFLAGS += -g -O0
else    
	CXXFLAGS += -O2 -DNDEBUG
endif

SRCS = main.cpp renderer.cpp mesh.cpp glfont.cpp utils.cpp

PROEJCT = demo

HOSTNAME = $(shell cat /etc/resolv.conf | grep nameserver | awk '{print $$2}')

all: build

build:
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(PROEJCT)

run:
	DISPLAY=$(HOSTNAME):0 LD_LIBRARY_PATH=$(OPENGL_PATH):$(LD_LIBRARY_PATH) ./$(PROEJCT)

clean:
	-rm -f $(PROEJCT) *.o