CC = gcc
CFLAGS = -Wall 
FRAMEWORK = -F /Library/Frameworks -framework SDL2

build: main.c
	$(CC) $(CFLAGS) main.c -o main $(FRAMEWORK) 

run: main
	./main

clean:
	rm -f main
