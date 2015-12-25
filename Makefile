greed: greed.o room.o output.o
	c++ -o greed greed.o room.o output.o -std=c++14 -Wall -Werror -lncurses
greed.o: greed.cpp
	c++ -c greed.cpp -std=c++14 -Wall -Werror
room.o: room.cpp
	c++ -c room.cpp -std=c++14 -Wall -Werror
output.o: output.cpp
	c++ -c output.cpp -std=c++14 -Wall -Werror
clean:
	rm -- *.o
	rm greed
