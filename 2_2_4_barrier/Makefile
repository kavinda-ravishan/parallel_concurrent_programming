CXX		 = g++
CXXFLAGS = -Wall -std=c++20
TARGET	 = main
SRC		 = main.cpp
LDLIBS	 = -lboost_thread -lboost_system
LIBS     = /usr/local/lib
INC_DIRS = /usr/local/include/boost

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDLIBS) -o $(TARGET) -I ${INC_DIRS} -L ${LIBS}

.PHONY: clean
clean:
	rm -f $(TARGET)