CXXFLAGS += -std=c++17 -fPIC -Wall -Wextra -pedantic -Wfatal-errors
CXXFLAGS += -I../../third_party/cocogfx/include

# Debugigng
ifdef DEBUG
	CXXFLAGS += -g -O0
else    
	CXXFLAGS += -O2 -DNDEBUG
endif

PROJECT = libshared.a

SRCS = $(wildcard *.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJS)
	ar rcs $@ $^

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend;

clean:
	rm -rf *.a *.o .depend

ifneq ($(MAKECMDGOALS),clean)
    -include .depend
endif