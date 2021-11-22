CXXFLAGS += -std=c++17 -fPIC -O3 -Wall -Wextra -pedantic -Wfatal-errors -I../../shared -I../../include
#CXXFLAGS += -std=c++17 -fPIC -O0 -g -DCOCOGL_API_PROFILE -Wall -Wextra -pedantic -Wfatal-errors -I../../shared -I../../include

LDFLAGS += -shared -L../../shared -lshared

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