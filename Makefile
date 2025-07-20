# Makefile for Aufgabe 4

CXX = g++
CXXFLAGS = -I. -I/usr/local/include -std=c++17 -Wall -Wextra
GTEST_LIBS = /usr/local/lib/libgtest_main.a /usr/local/lib/libgtest.a
PTHREAD_LIB = -lpthread

# Source files (excluding main files and Qt files)
SOURCES = network.cpp csv.cpp scheduled_trip.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: test_runner

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target for automatic testing
autotest: test_runner
	./test_runner

# Build test runner
test_runner: $(OBJECTS) tester.cpp
	$(CXX) $(CXXFLAGS) -o test_runner $(GTEST_LIBS) tester.cpp $(SOURCES) $(PTHREAD_LIB)

# Build main application (console version with iterators)
main_app: $(OBJECTS) main.cpp
	$(CXX) $(CXXFLAGS) -o main_app main.cpp $(SOURCES)

# Test main application with sample data
test_main: main_app
	./main_app ./gtfsshort

# Clean build files
clean:
	rm -f *.o test_runner main_app

.PHONY: all autotest test_main clean