SOURCE=src/main.c src/game.c src/scores.c src/pipes.c

main: $(SOURCE)
	gcc -o main $(SOURCE) -lraylib -lcjson -Wall -Wextra -Wpedantic -O3

clean: main
	rm main
