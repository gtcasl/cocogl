
CXXFLAGS += -std=c++17 -fPIC -O0 -g -Wall -Wextra -pedantic -Wfatal-errors
LDFLAGS += 

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