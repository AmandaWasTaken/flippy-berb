SOURCE=src/*.c

main: $(SOURCE)
	gcc -o main $(SOURCE) -lraylib -lcjson -Wall -Wextra -Wpedantic -O3

clean: main
	rm main
