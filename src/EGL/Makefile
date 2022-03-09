CXXFLAGS += -std=c++17 -fPIC -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../shared -I../../include -I../../third_party/cocogfx/include

LDFLAGS += -shared -L../shared -lshared

BIN_DIR = ../../bin

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

PROJECT = libEGL.so

SRCS = $(wildcard *.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(BIN_DIR) $(BIN_DIR)/$(PROJECT)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/$(PROJECT): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend;

clean:
	rm -rf $(BIN_DIR)/$(PROJECT) *.o .depend

ifneq ($(MAKECMDGOALS),clean)
    -include .depend
endif