CXX = g++
CXXFLAGS = -I. -I/usr/local/include -std=c++17
LDFLAGS = /usr/local/lib/libgtest_main.a /usr/local/lib/libgtest.a
SOURCES = network.cpp scheduled_trip.cpp csv.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = test_runner

# Default target
all: $(TARGET)

# Build the test runner
autotest: $(TARGET)

$(TARGET): $(OBJECTS) tester.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(LDFLAGS) tester.cpp $(OBJECTS)

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Build Qt application
qt: project.pro
	qmake project.pro
	make

.PHONY: all clean autotest qt