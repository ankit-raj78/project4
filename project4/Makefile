CC=g++
CFLAGS=-O3 -std=c++17 -Wall
# Get actual paths from brew
PG_PATH=$(shell brew --prefix libpq)
PQXX_PATH=$(shell brew --prefix libpqxx)

# Include and library paths
INCLUDES=-I$(PG_PATH)/include -I$(PQXX_PATH)/include
EXTRAFLAGS=-L$(PG_PATH)/lib -L$(PQXX_PATH)/lib -lpqxx -lpq

all: test

# Compile each file separately to better isolate errors
test: main.o exerciser.o query_funcs.o
	$(CC) $(CFLAGS) -o test main.o exerciser.o query_funcs.o $(EXTRAFLAGS)

main.o: main.cpp query_funcs.h exerciser.h
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp

exerciser.o: exerciser.cpp exerciser.h query_funcs.h
	$(CC) $(CFLAGS) $(INCLUDES) -c exerciser.cpp

query_funcs.o: query_funcs.cpp query_funcs.h
	$(CC) $(CFLAGS) $(INCLUDES) -c query_funcs.cpp

clean:
	rm -f *~ *.o test

clobber:
	rm -f *~ *.o
