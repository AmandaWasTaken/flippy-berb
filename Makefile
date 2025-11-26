SOURCE=main.c game.c 

main: main.c
	gcc -o main $(SOURCE) -lraylib -Wall -Wextra -Wpedantic -O3
