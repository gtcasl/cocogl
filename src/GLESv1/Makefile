CXXFLAGS += -std=c++17 -fPIC -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../../shared -I../../include

LDFLAGS += -shared -L../../shared -lshared

# Debugigng
ifdef DEBUG
	CXXFLAGS += -g -O0
else    
	CXXFLAGS += -O2 -DNDEBUG
endif

# API trace
ifdef APITRACE
	CXXFLAGS += -DCOCOGL_API_TRACE
endif

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