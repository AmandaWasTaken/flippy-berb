SOURCE=main.c game.c scores.c

main: $(SOURCE)
	gcc -o main $(SOURCE) -lraylib -lcjson -Wall -Wextra -Wpedantic -O3
