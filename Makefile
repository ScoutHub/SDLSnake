CC = gcc
CFLAGS = -Wall 
FRAMEWORK = -F /Library/Frameworks -framework SDL2
LFLAGS = `sdl2-config --cflags --libs`

build_mac:
	$(CC) $(CFLAGS) main.c -o main $(FRAMEWORK) 

build_linux:
	$(CC) main.c -o main $(LFLAGS) 

run: main
	./main

clean:
	rm -f main
