CC = g++
NAME = terminal3Dness
CXXFLAGS = -o $(NAME) -std=c++20 -lncursesw

all:
	$(CC) *.cpp $(CXXFLAGS)
