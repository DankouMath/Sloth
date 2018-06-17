all:clean build

sdl:
	sudo apt-get install libsdl1.2-dev
	sudo apt-get install libsdl-ttf2.0-dev
	
build:sdl
	sudo gcc -c sources/sloth.c -o sources/sloth.o
	sudo ar rc libsloth.a sources/sloth.o
	sudo rm -f sources/sloth.o
clean:
	sudo rm -f *.a .*o *.out
