platform: Administator.o Calculator.o Database.o main.o terminal.o
	g++-10 -std=c++20 Administator.o Calculator.o Database.o main.o terminal.o -o platform
	./platform

Administator.o: Database.h terminal.h Administator.h Administator.cpp
	g++-10 -std=c++20 -c Administator.cpp -o Administator.o

Calculator.o: Calculator.h Calculator.cpp
	g++-10 -std=c++20 -c Calculator.cpp -o Calculator.o

Database.o: Database.h terminal.h Database.cpp
	g++-10 -std=c++20 -c Database.cpp -o Database.o

main.o: main.h Administator.h Database.h Calculator.h terminal.h main.cpp
	g++-10 -std=c++20 -c main.cpp -o main.o

terminal.o: terminal.h terminal.cpp
	g++-10 -std=c++20 -c terminal.cpp -o terminal.o

clean:
	rm platform Administator.o Calculator.o Database.o main.o terminal.o

cleantxt:
	rm *.txt
