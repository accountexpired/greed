greed: greed.o
	c++ -o greed greed.o -std=c++11 -Wall -Werror
greed.o: greed.cpp
	c++ -c greed.cpp -std=c++11 -Wall
