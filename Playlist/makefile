# macros
FLAGS = -g -W -Wall

# make the main program
playlist: playlist.o main.o
	g++ $(FLAGS) playlist.o main.o -o playlist
	@echo "All done"

# make the playlist object file
playlist.o: playlist.cpp playlist.h
	g++ $(FLAGS) -c playlist.cpp

# make the main object file
main.o: main.cpp playlist.h
	g++ $(FLAGS) -c main.cpp

# delete unneeded files
clean:
	rm -f playlist *.o

