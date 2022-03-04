CXXFLAGS += -std=c++17 -fPIC -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../shared -I../../include -I../../third_party/cocogfx/include

LDFLAGS += -shared -L../shared -lshared -L../../third_party/cocogfx -lcocogfx

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

PROJECT = libGLES_CM.so

SRCS = $(wildcard ../GLESv1/*.cpp ../EGL/*.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend;

clean:
	rm -rf *.so ../EGL/*.o ../GLESv1/*.o .depend

ifneq ($(MAKECMDGOALS),clean)
    -include .depend
endif