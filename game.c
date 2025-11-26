#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>
#include "game.h"


Pipe_pair spawn_pipe_pair(Window* window){

	const int gap_size = 200;
	const int error_margin = 20;

	Texture2D top_tex = LoadTexture("assets/pipe_top.png");
	Texture2D bot_tex = LoadTexture("assets/pipe_bot.png");

	int min_gap = error_margin;
	int max_gap = window->h - gap_size - error_margin;

	int gap_y = GetRandomValue(min_gap, max_gap);

	Pipe top = {
		.sprite = top_tex, 
		.posX = window->w,
		.posY = gap_y - top.sprite.height, 
		.tint = WHITE,
	};

	Pipe bot = {
		.sprite = bot_tex, 
		.posX = window->w,
		.posY = gap_y + gap_size, 
		.tint = WHITE,
	};

	Pipe_pair ret = {
		.top = top,
		.bot = bot,
		.exists = true,
	};

	return ret;
}

void game_over(Window* window){
	
	char gameOver[11] = "Game Over!";
	const size_t len = strlen(gameOver);
	DrawText(gameOver, window->w - len/2, 500, 40, RED);
	EndDrawing();
}

bool check_env_collision(Window* window, Bird* bird){
	return bird->posY > window->h || bird->posY < 0;
}


bool check_pipe_collision(Window* window, Bird* bird, Pipe_pair* pair){

	// TODO 
}

void poll_events(Bird* bird){

	if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)){
		bird->velocityY = -bird->jump_velocity;
	}	
} 

void event_loop(Bird* bird, Window* window){

	Pipe_pair pair = {0};
	pair.exists = false;
	float pipe_mvmt = 2.75f; // movement speed of pipes

	while(!WindowShouldClose()){
		
	BeginDrawing();

		if(check_env_collision(window, bird)){
			fprintf(stdout, "YOU DIED!!!!!!!!!\n");
			game_over(window);
		}
		
		int should_spawn_pipes = rand() % 5;	

		if(should_spawn_pipes == 0 && !pair.exists){
			pair = spawn_pipe_pair(window);
			fprintf(stdout, "Pair Spawned\n");
		}

		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);

		poll_events(bird);

		// shits backwards; bird falls down with +=
		bird->velocityY += bird->gravity;
		bird->posY += bird->velocityY;

		char pipe_speed[32];
		sprintf(pipe_speed, "Current speed: %f", pipe_mvmt);
		pipe_speed[strlen(pipe_speed)] = '\0';
		DrawText(pipe_speed, 200, window->h - 32, 30, RED);

		DrawTexture(bird->sprite,
			    bird->posX,
			    bird->posY,
			    bird->tint);

		if(pair.exists){
			pair.top.posX -= pipe_mvmt;
			DrawTexture(pair.top.sprite,
		    		pair.top.posX,
    		    		pair.top.posY,
			    	pair.top.tint);

			pair.bot.posX -= pipe_mvmt;
			DrawTexture(pair.bot.sprite,
		   		pair.bot.posX,
			   	pair.bot.posY,
		   		pair.bot.tint);

			// Ready to spawn a new pair of pipes
			// (Previous pair has despawned)
			if(pair.top.posX + pair.top.sprite.width < 0){
				pair.exists = false;
				fprintf(stdout, "Pair Despawned\n");
				pair.exists = false;
				pipe_mvmt += 0.125f;
			}
		}

	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	CloseWindow();
}
