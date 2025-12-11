SOURCE=main.c game.c 

main: $(SOURCE)
	gcc -o main $(SOURCE) -lraylib -Wall -Wextra -Wpedantic -O3
