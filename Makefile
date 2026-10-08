# Name: Miriam Valenzuela
# Red ID: 131786119

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g3 -pthread

OBJS = main.o log.o 

PROGRAM = chargebay

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(OBJS)

main.o: main.cpp shared.h log.h
	$(CXX) $(CXXFLAGS) -c main.cpp

log.o: log.cpp log.h
	$(CXX) $(CXXFLAGS) -c log.cpp

clean:
	rm -f *.o $(PROGRAM)