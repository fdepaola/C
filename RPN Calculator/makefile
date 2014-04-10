FLAGS = -W -Wall

calculator: calc.o main.o
	g++ $(FLAGS) calc.o main.o -o calculator
	@echo "WE DID IT!"

calc.o: calc.cpp calc.h
	g++ $(FLAGS) -c calc.cpp

main.o: main.cpp calc.h
	g++ $(FLAGS) -c main.cpp

clean:
	rm -f calculator *.o
