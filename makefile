
all: clean compile link

link:
	g++ -o bin/warehouse bin/main.o bin/WareHouse.o bin/Parse.o bin/Volunteer.o bin/Customer.o bin/Order.o bin/Action.o

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Parse.o src/Parse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

clean:
	rm -f bin/*
