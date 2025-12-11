#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>
#include "game.h"


int main(int argc, char** argv){

	// For "random" pipe spawning 
	srand(time(NULL));

	Window game = {
		.title = "Birb",
		.w = 1000,
		.h = 700,
		.attempts = 1,
	};

	InitWindow(game.w, game.h, game.title);
	SetTargetFPS(60);

	Bird bird = {
		.sprite = LoadTexture("assets/birb3.png"),
		.posX = 300,
		.posY = 300,
		.gravity = 0.45f,
		.jump_velocity = 7.5f,
		.velocityY = 0.0f,
		.tint = WHITE
	};

//	Bird bird = DEFAULT_BIRD;
//	bird.sprite = LoadTexture("assets/birb3.png");
	event_loop(&bird, &game);
        return 0;
}

