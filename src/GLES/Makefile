#CXX = clang
CXXFLAGS += -std=c++17 -fPIC -O0 -g -Wall -Wextra -pedantic -Wfatal-errors -I../../shared -I../../include
LDFLAGS += -shared -L../../shared -lshared

PROJECT = libGLESv1_CM.so

SRCS = $(wildcard *.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend;

clean:
	rm -rf *.so *.o .depend

ifneq ($(MAKECMDGOALS),clean)
    -include .depend
endif