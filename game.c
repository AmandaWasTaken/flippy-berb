#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "scores.h"

#define INITIAL_PIPE_SPEED 2.75f
#define FONT_SIZE_GENERAL 22
#define FONT_SIZE_GAMEOVER 40

Bird DEFAULT_BIRD = {	
	.posX = 300,
	.posY = 300,
	.gravity = 0.45f,
	.jump_velocity = 7.5f,
	.velocityY = 0.0f,
	.tint = WHITE,
};

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

		Rectangle bird_hitbox 	  = {0};
		Rectangle top_pipe_hitbox = {0};
		Rectangle bot_pipe_hitbox = {0};

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
			running = false;
		}
		
		if(CheckCollisionRecs(bird_hitbox, top_pipe_hitbox)){
			running = false;
		}

		if(CheckCollisionRecs(bird_hitbox, bot_pipe_hitbox)){
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

		/* Draw score */
		char current_score[32];
		sprintf(current_score, "Score: %i", window->score);

		int score_text_w = MeasureText(current_score, FONT_SIZE_GENERAL);
		int score_text_x = (GetScreenWidth() - score_text_w) / 2;
		DrawText(current_score, score_text_x, 0 + 32, FONT_SIZE_GENERAL, RED);

		/* Draw Attempt */
		char current_attempt[16];
		sprintf(current_attempt, "Attempt: %i", window->attempts);

		int att_text_w = MeasureText(current_attempt, FONT_SIZE_GENERAL);
		int att_text_x = (GetScreenWidth() - att_text_w) / 2;
		DrawText(current_attempt, att_text_x, 32 + 24, FONT_SIZE_GENERAL, RED);

		/* Draw top scores (max. 5) */ 
		int scores[5]; 
		int score_count = fetch_scores(scores);
	    	qsort(scores, score_count, sizeof(int), compare);

		char sd[5 * 5];
		sprintf(sd, "%i\n%i\n%i\n%i\n%i", scores[0], scores[1], scores[2],
				scores[3], scores[4]);

		sd[strlen(sd)] = '\0';
		DrawText(sd, window->w - 100, 10, 20, RED);

		char score_pos[20];
		sprintf(score_pos, "#1:\n#2:\n#3:\n#4:\n#5:");
		score_pos[strlen(score_pos)] = '\0';
		DrawText(score_pos, window->w - 150, 10, 20, MAROON);

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


		/* Game over */
		} else { 
			char* game_over = "Game Over!";
			int game_over_w = MeasureText(game_over, FONT_SIZE_GAMEOVER);
			int game_over_x = (GetScreenWidth() - game_over_w) / 2;
			int game_over_y = GetScreenHeight()/2 - 200;
			DrawText(game_over, game_over_x, game_over_y, FONT_SIZE_GAMEOVER, RED);


			char final_score[32];
			sprintf(final_score, "Final score: %i", window->score); 
			int score_w = MeasureText(final_score, FONT_SIZE_GAMEOVER);
			int score_x = (GetScreenWidth() - score_w) / 2;
			int score_y = GetScreenHeight()/2 - 100;
			DrawText(final_score, score_x, score_y, FONT_SIZE_GAMEOVER, RED);

			char restart[19];
			sprintf(restart, "Press R to Restart");
			int res_w = MeasureText(restart, FONT_SIZE_GAMEOVER);
			int res_x = (GetScreenWidth() - res_w) / 2;
			int res_y = GetScreenHeight()/2;
			DrawText(restart, res_x, res_y, FONT_SIZE_GAMEOVER, DARKGREEN);

			/* Restart game */
			if(IsKeyPressed('R')){

				/* Write score to db */
				save_score(window->score);

				/* Reset score */
				window->score = 0;
				window->attempts++;

				/* Reset birb */
				*bird = DEFAULT_BIRD;
				bird->sprite = LoadTexture("assets/birb3.png");

				/* Reset pipes */
				pair.exists = false;
				pair.counted = false;
				pair.top.posX = 0;
				pair.top.posY = 0;
				pair.bot.posX = 0;
				pair.bot.posY = 0;
				pipe_mvmt = INITIAL_PIPE_SPEED;

				/* we go agane */
				running = true;
			}
		}	

	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	UnloadTexture(pair.top.sprite);
	UnloadTexture(pair.bot.sprite);
	CloseWindow();
}
