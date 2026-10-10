# Name: Miriam Valenzuela
# Red ID: 131786119

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g3 -pthread

OBJS = main.o dispatcher.o log.o

PROGRAM = chargebay

$(PROGRAM): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(OBJS)

main.o: main.cpp shared.h dispatcher.h log.h
	$(CXX) $(CXXFLAGS) -c main.cpp

dispatcher.o: dispatcher.cpp dispatcher.h shared.h log.h
	$(CXX) $(CXXFLAGS) -c dispatcher.cpp

log.o: log.cpp log.h
	$(CXX) $(CXXFLAGS) -c log.cpp

clean:
	rm -f *.o $(PROGRAM)