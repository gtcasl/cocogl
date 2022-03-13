CXXFLAGS += -std=c++17 -fPIC -Wall -Wextra -pedantic -Wfatal-errors 
CXXFLAGS += -I../shared -I../../include -I../../third_party/cocogfx/include

LDFLAGS += -shared -L../shared -lshared -L../../third_party/cocogfx -lcocogfx -lpng -lz -lboost_serialization

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

PROJECT = libGLESv1_CM.so

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