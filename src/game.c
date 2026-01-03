#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <string.h>
#include <time.h>

#include "../lib/game.h"
#include "../lib/scores.h"
#include "../lib/pipes.h"
#include "../lib/bg.h"
#include "../lib/mainmenu.h"

Bird DEFAULT_BIRD = {	
	.posX = 300,
	.posY = 300,
	.gravity = 0.45f,
	.jump_velocity = 7.5f,
	.velocityY = 0.0f,
};



void update_score(Pipe_buffer* buf, Bird* bird, int* score, float* speed){

	for(int i = 0; i < buf->count; i++){
		int idx = (buf->tail + i) % MAX_PIPES;
		Pipe_pair* p = &buf->pairs[idx];

		if(!p->counted &&
		   bird->posX > p->top.posX + PIPE_WIDTH){
			(*score)++;
			p->counted = true;
			(*speed) += 0.1f;
		}
	}
}
void render_score(Window* window){

		char current_score[32];
		sprintf(current_score, "Score: %i", window->score);

		int score_text_w = MeasureText(current_score, FONT_SIZE_GENERAL);
		int score_text_x = (GetScreenWidth() - score_text_w) / 2;
		DrawText(current_score, score_text_x, 0 + 32, FONT_SIZE_GENERAL, RED);
}

void render_attempt(Window* window){

		char current_attempt[16];
		sprintf(current_attempt, "Attempt: %i", window->attempts);

		int att_text_w = MeasureText(current_attempt, FONT_SIZE_GENERAL);
		int att_text_x = (GetScreenWidth() - att_text_w) / 2;
		DrawText(current_attempt, att_text_x, 32 + 24, FONT_SIZE_GENERAL, RED);
}

void render_leaderboard(Window* window){

		int scores[10]; 
		int score_count = fetch_scores(scores);
	    	qsort(scores, score_count, sizeof(int), _compare);

		char score_display[score_count * 16];
		char* wp = score_display;
		int remaining = sizeof(score_display);
		int entries = score_count;

		for(int i = 0; i < entries; i++){
			int written = snprintf(wp, remaining, "%i\n", scores[i]);
			if(written < 0 || written >=remaining) break;

			wp += written;
			remaining -= written;
		}
		DrawText(score_display, window->w - 25, 10, 30, MAROON);

		char score_pos[256];
		wp = score_pos;
		remaining = sizeof(score_pos);
		entries = score_count;

		for(int i = 1; i <= entries; i++){
			int written = snprintf(wp, remaining, "#%i:\n", i);
			if(written < 0 || written >=remaining) break;

			wp += written;
			remaining -= written;
		}
		DrawText(score_pos, window->w - 95, 10, 30, MAROON);

}

bool check_env_collision(Window* window, Bird* bird){
	return bird->posY > window->h || bird->posY < 0;
}

void game_reset(Window* window, Bird* bird, Pipe_buffer* buf, 
		Texture2D bird_sprite, Texture2D top, Texture2D bot){

	window->score = 0;
	*bird = DEFAULT_BIRD;
	bird->sprite = bird_sprite;
	bird->tint = WHITE;
	pipebuf_init(buf);

	float start_x = GetScreenWidth() + 100.0f;
	for(int i = 0; i < MAX_PIPES; i++){
		pipebuf_append(buf, start_x + i * PIPE_SPACING,
				top, bot);
	}
}

void draw_debug_info(Window* window, Bird* bird, 
		bool* show_hitboxes, float pipe_speed){

	char hitbox_status[32];
	sprintf(hitbox_status, "%s", show_hitboxes ? 
				"Hitboxes: Visible" :
				"Hitboxes: Hidden");
	
	char curr_speed[32];
	sprintf(curr_speed, "Speed %.2f", pipe_speed);
	curr_speed[strlen(curr_speed)] = '\0';
	DrawText(curr_speed, 200, window->h - 128, 30, RED);
	
	hitbox_status[strlen(hitbox_status)] = '\0';
	DrawText(hitbox_status, 200, window->h - 64, 30, RED);
	DrawText("(H to toggle)", 600, window->h - 64, 30, RED);
	if(show_hitboxes){
		// Render bird hitbox
		DrawRectangle(bird->posX,
			      bird->posY,
			      132, 88, RED); 
		}
}

void end_game(Window* window, bool* running, Bird* bird,
		Texture2D top, Texture2D bot, Texture2D bird_sprite,
		Pipe_buffer* buf){

	/* Draw game over textg */
	char* game_over = "Game Over!";
	int game_over_w = MeasureText(game_over, FONT_SIZE_GAMEOVER);
	int game_over_x = (GetScreenWidth() - game_over_w) / 2;
	int game_over_y = GetScreenHeight()/2 - 200;
	DrawText(game_over, game_over_x, game_over_y, FONT_SIZE_GAMEOVER, RED);

	/* Draw final score */
	char final_score[32];
	sprintf(final_score, "Final score: %i", window->score); 
	int score_w = MeasureText(final_score, FONT_SIZE_GAMEOVER);
	int score_x = (GetScreenWidth() - score_w) / 2;
	int score_y = GetScreenHeight()/2 - 100;
	DrawText(final_score, score_x, score_y, FONT_SIZE_GAMEOVER, RED);

	/* Proompt for restart */
	char restart[19];
	sprintf(restart, "Press R to Restart");
	int res_w = MeasureText(restart, FONT_SIZE_GAMEOVER);
	int res_x = (GetScreenWidth() - res_w) / 2;
	int res_y = GetScreenHeight()/2;
	DrawText(restart, res_x, res_y, FONT_SIZE_GAMEOVER, DARKGREEN);

	/* Erase scores? */

	char erase[14];
	sprintf(erase, "Erase scores?");
	int y2 = GetScreenHeight()/2 - 50;
	DrawText(erase, res_x, y2, FONT_SIZE_GENERAL, RED);

	if(IsKeyPressed('Q')){
		fprintf(stdout, "Erasing scores. . .\n");
		reset_scores();
	}

	/* Restart game */
	if(IsKeyPressed('R')){

		/* Write score to db */
		if(window->score > 0) {
			save_score(window->score);
			fprintf(stdout, "Saving score. . .\n");
		} else {
			fprintf(stdout, "Score of %i was not saved. "
					"(Less than or equal to 0)\n",
					window->score);
		}

		/* Reset score */
		window->attempts++;

		/* Reset game state */
		game_reset(window, bird, buf,
				bird_sprite, top, bot);
		
		/* we go agane */
		*running = true;
	}
}

void poll_events(Bird* bird, bool* show_debug, bool* show_hitboxes){

	if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP)){
		bird->velocityY = -bird->jump_velocity;
	}	
	if(IsKeyPressed('H')){
		*show_hitboxes = ! *show_hitboxes;
	}

	if(IsKeyPressed('D')){
		*show_debug = ! *show_debug;
	}
} 

void event_loop(Bird* bird, Window* window){


	// Load all textures 
	const Texture2D top_sprite 	= LoadTexture("assets/pipe_top.png");
	const Texture2D bot_sprite  	= LoadTexture("assets/pipe_bot.png");
	const Texture2D bird_sprite 	= LoadTexture("assets/birb3.png");
	const Texture2D bg_texture 	= LoadTexture("assets/bg.png");
	const Texture2D b_start_sprite  = LoadTexture("assets/start.png");
	const Texture2D b_quit_sprite 	= LoadTexture("assets/quit.png");
	const Texture2D b_scores_sprite = LoadTexture("assets/leaderboard.png");
	const Texture2D menu_bg		= LoadTexture("assets/menu_bg.png");


	bool running 	   = false;
	bool show_hitboxes = false;
	bool show_debug    = false;
	bool first_launch  = true;

	float bg_offset = 0.0f;
	/* How much the bg texture moves each frame */
	const float bg_parallax_factor = 0.35f;
	

	/* Initialize pipe buffer */
	Pipe_buffer pipe_buf;
	pipebuf_init(&pipe_buf);
	float pipe_speed = INITIAL_PIPE_SPEED;
	float start_x = GetScreenWidth() + 100.0f;
	const int dt = 1;
	for(int i = 0; i < MAX_PIPES; i++){
		pipebuf_append(&pipe_buf, start_x + i * PIPE_SPACING, 
			       top_sprite, bot_sprite);
	}

	while(!WindowShouldClose()){
		
	ClearBackground(RAYWHITE);
	DrawFPS(10, 10);

	BeginDrawing();

		/* Hitbox position settings */

		Rectangle bird_hitbox 	= {0};

		bird_hitbox.x 	   	= bird->posX;
		bird_hitbox.y 	   	= bird->posY;
		bird_hitbox.width  	= 132;
		bird_hitbox.height 	= 88;

		/* Render main menu before game starts */ 
		if(first_launch)
			render_menu(window, &running, 
				    &first_launch, b_start_sprite,
				    b_quit_sprite, b_scores_sprite,
				    menu_bg);

		/* Main Loop */
		if(running){


		/* Render background */
		render_background(window, bg_texture, &bg_offset);
		bg_offset += pipe_speed * bg_parallax_factor;

		/* Render pipes */
		pipebuf_update(&pipe_buf, pipe_speed, dt, top_sprite, bot_sprite);
		pipebuf_render(&pipe_buf);


		/* Check collisions */
		if(check_env_collision(window, bird)){
			running = false;
		}
		
		if(check_pipe_collision(&pipe_buf.pairs[pipe_buf.tail], bird_hitbox)){
			running = false;
		}

		/* Poll Events */
		poll_events(bird, &show_hitboxes, &show_debug);
		bird->velocityY += bird->gravity;
		bird->posY += bird->velocityY;		

		/* Increment score */
		update_score(&pipe_buf, bird, &window->score, &pipe_speed);

		/* Draw score */
		render_score(window);

		/* Draw Attempt */
		render_attempt(window);

		/* Draw top 10 scores */
		render_leaderboard(window);
			
		
		/* DEBUG */
		if(show_debug){
			if(IsKeyPressed('H')) show_hitboxes = !show_hitboxes;
			draw_debug_info(window, bird, 
					&show_hitboxes, pipe_speed);
		} else {
			DrawText("Press D to toggle debug info",
				 window->w - 900 , window->h - 50, 20, BLUE);
		}

		/* Render bird itself */
		DrawTexture(bird->sprite,
			    bird->posX,
			    bird->posY,
			    bird->tint);

		/* Game over */
		} else { 
			if(!first_launch){
				end_game(window, &running, bird, 
					 top_sprite, bot_sprite,
					 bird_sprite, &pipe_buf);
			}	
		}
	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	UnloadTexture(top_sprite);	
	UnloadTexture(bot_sprite); 		
	UnloadTexture(bird_sprite);	
	UnloadTexture(bg_texture);		
	UnloadTexture(b_start_sprite); 
	UnloadTexture(b_quit_sprite);	
	UnloadTexture(b_scores_sprite);
	UnloadTexture(menu_bg);

	CloseWindow();
}
