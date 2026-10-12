CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

OBJS = main.o Cola.o Lector.o

hospital: $(OBJS)
	$(CXX) $(CXXFLAGS) -o hospital $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o hospital
