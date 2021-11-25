#CXXFLAGS += -std=c++11 -fPIC -O3 -Wall -Wextra -Wfatal-errors
CXXFLAGS += -std=c++11 -fPIC -O0 -g -Wall -Wextra -Wfatal-errors

CXXFLAGS += -DLUPNG_USE_ZLIB -Iinclude

LDFLAGS += 

PROJECT = libcocogfx.a

SRCS = $(wildcard src/*.cpp)

OBJS := $(SRCS:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJS)
	ar rcs $@ $^

.depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $^ > .depend;

clean:
	rm -rf *.a src/*.o .depend

ifneq ($(MAKECMDGOALS),clean)
    -include .depend
endif