CXX=g++
CXXFLAGS=-O3 -std=c++17 -Wall
LDFLAGS=-lpqxx -lpq

# PostgreSQL libraries and headers should be in standard locations
# after installing libpqxx-dev on Ubuntu

SRCS=main.cpp query_funcs.cpp exerciser.cpp
OBJS=$(SRCS:.cpp=.o)
EXEC=test

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(EXEC)
