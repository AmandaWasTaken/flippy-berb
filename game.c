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
		.counted = false,
	};

	return ret;
}
/*
void game_over(Window* window){
	
	char gameOver[11] = "Game Over!";
	const size_t len = strlen(gameOver);

	BeginDrawing();
	while(1){
		DrawText(gameOver, window->w - len/2, 500, 40, RED);
		if(IsKeyPressed(KEY_ESCAPE)) break;
	}
	return;
}
*/
bool check_env_collision(Window* window, Bird* bird){
	return bird->posY > window->h || bird->posY < 0;
}


void poll_events(Bird* bird, bool* show_hitboxes){

	if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)){
		bird->velocityY = -bird->jump_velocity;
	}	
	if(IsKeyPressed('H')){
		*show_hitboxes = ! *show_hitboxes;
	}
} 

void event_loop(Bird* bird, Window* window){

	Rectangle bird_hitbox = {0};
	Rectangle top_pipe_hitbox = {0};
	Rectangle bot_pipe_hitbox = {0};

	bool running = true;
	bool show_hitboxes = false;
	
	Pipe_pair pair = {0};
	pair.exists = false;
	float pipe_mvmt = 2.75f; // movement speed of pipes
	
	while(!WindowShouldClose()){
		
	ClearBackground(RAYWHITE);
	DrawFPS(10, 10);

	BeginDrawing();

		/* Hitbox position settings */

		bird_hitbox.x 	   	= bird->posX;
		bird_hitbox.y 	   	= bird->posY;
		bird_hitbox.width  	= 132;
		bird_hitbox.height 	= 88;

		top_pipe_hitbox.x 	= pair.top.posX;
		top_pipe_hitbox.y 	= pair.top.posY;
		top_pipe_hitbox.width 	= 200;
		top_pipe_hitbox.height 	= 600;

		bot_pipe_hitbox.x 	= pair.bot.posX;
		bot_pipe_hitbox.y 	= pair.bot.posY;
		bot_pipe_hitbox.width 	= 200;
		bot_pipe_hitbox.height 	= 600;


		/* Main Loop */
		if(running){

		/* Check collisions */
		if(check_env_collision(window, bird)){
			fprintf(stdout, "YOU DIED!!!!!!!!!\n");
			running = false;
		}
		
		if(CheckCollisionRecs(bird_hitbox, top_pipe_hitbox)){
			fprintf(stdout, "Hit top pipe!!!!\n");
			running = false;
		}

		if(CheckCollisionRecs(bird_hitbox, bot_pipe_hitbox)){
			fprintf(stdout, "Hit bot pipe!!!!\n");
			running = false;
		}

		/* Poll Events */
		poll_events(bird, &show_hitboxes);
		bird->velocityY += bird->gravity;
		bird->posY += bird->velocityY;

		/* Spawn pipes */
		int should_spawn_pipes = rand() % 5;	
		if(should_spawn_pipes == 0 && !pair.exists){
			pair = spawn_pipe_pair(window);
			fprintf(stdout, "Pair Spawned\n");
		}

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

			if(bird->posX > pair.top.posX && !pair.counted){
				pair.counted = true;
				window->score++;
			}

			// Ready to spawn a new pair of pipes
			// (Previous pair has despawned)
			if(pair.top.posX + pair.top.sprite.width < 0){
				pair.exists = false;
				fprintf(stdout, "Pair Despawned\n");
				pipe_mvmt += 0.375f;
			}
		}

		/* Do stuff if there's a pipe pair on screen */
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
				pipe_mvmt += 0.375f;
			}
		}

		char current_score[32];
		sprintf(current_score, "Score: %i", window->score);
		current_score[strlen(current_score)] = '\0';
		DrawText(current_score, window->w / 2 - strlen(current_score),
				0 + 32, 30, RED);


		/* DEBUG */
		char pipe_speed[32];
		sprintf(pipe_speed, "Current speed: %f", pipe_mvmt);
		pipe_speed[strlen(pipe_speed)] = '\0';
		DrawText(pipe_speed, 200, window->h - 32, 30, RED);

		/* DEBUG */
		char hitbox_status[32];
		sprintf(hitbox_status, "%s", show_hitboxes ? "Hitboxes: Visible" :
							     "Hitboxes: Hidden");

		hitbox_status[strlen(hitbox_status)] = '\0';
		DrawText(hitbox_status, 200, window->h - 64, 30, RED);
		DrawText("(H to toggle)", 600, window->h - 64, 30, RED);
		if(show_hitboxes){

			// Render bird hitbox
			DrawRectangle(bird->posX,
				      bird->posY,
				      132, 88, RED); 
			if(pair.exists){
				// Render pipe hitboxes
				DrawRectangle(pair.top.posX,
					      pair.top.posY,
					      200, 600, RED);
				DrawRectangle(pair.bot.posX,
					      pair.bot.posY,
					      200, 600, RED);
			}
		}

		DrawTexture(bird->sprite,
			    bird->posX,
			    bird->posY,
			    bird->tint);

		} else { 
			DrawText("GAME OVER", window->w/2 - 150, window->h/2,
				 50, RED);

			char final_score[32];
			sprintf(final_score, "Final score: %i", window->score); 
			DrawText(final_score, window->w/2 - 150, window->h/2 + 70,
				 50, RED);
		}
		
	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	UnloadTexture(pair.top.sprite);
	UnloadTexture(pair.bot.sprite);
	CloseWindow();
}
