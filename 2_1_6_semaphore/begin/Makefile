# Semaphore Demo Makefile
CXX		 = g++
CXXFLAGS = -Wall -std=c++17
TARGET	 = semaphore_demo
SRC		 = semaphore_demo.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)