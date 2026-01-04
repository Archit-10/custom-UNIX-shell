CXX = g++
CXXFLAGS = -Wall -pedantic -Werror -g

SRCS = $(wildcard src/core/*.cpp src/commands/*.cpp src/features/*.cpp src/utilities/*.cpp)
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

Posix_Shell: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) Posix_Shell
