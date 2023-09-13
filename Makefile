CC = gcc
CFLAGS = -Wall 
FRAMEWORK = -F /Library/Frameworks -framework SDL2
LINUX_FLAGS = `sdl2-config --cflags --libs`

build_mac: main.c
	$(CC) $(CFLAGS) main.c -o main $(FRAMEWORK) 

build_linux:
	$(CC) main.c -o main $(LINUX_FLAGS) 

run: main
	./main

clean:
	rm -f main
