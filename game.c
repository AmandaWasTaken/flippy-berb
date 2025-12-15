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
#define PIPE_SPACING 500.0f

Bird DEFAULT_BIRD = {	
	.posX = 300,
	.posY = 300,
	.gravity = 0.45f,
	.jump_velocity = 7.5f,
	.velocityY = 0.0f,
	.tint = WHITE,
};

void pipebuf_init(Pipe_buffer* buf){
	buf->head  = 0;
	buf->tail  = 0;
	buf->count = 0;

	for(int i = 0; i < MAX_PIPES; i++){
		buf->pairs[i].exists = false;
		buf->pairs[i].counted = false;
	}
}

void spawn_pipe_pair(Pipe_pair* pair, float start_x, Texture2D top, Texture2D bot){

	float gap_y = GetRandomValue(150, 450);
	float gap_h = 225.0f;

	pair->top.sprite = top;
	pair->bot.sprite = bot;

	pair->top.posX = start_x;
	pair->top.posY = gap_y - gap_h/2 - PIPE_HEIGHT;

	pair->bot.posX = start_x;
	pair->bot.posY = gap_y + gap_h/2;

	pair->exists = true;
	pair->counted = false;

	pair->top.tint = WHITE;
	pair->bot.tint = WHITE;

	pair->hitbox = (Rectangle){
		start_x,
		0,
		PIPE_WIDTH,
		GetScreenHeight()
	};
}

bool pipebuf_spawn(Pipe_buffer* buf, float start_x, Texture2D top, Texture2D bot){
	
	if(buf->count == MAX_PIPES) return false;

	spawn_pipe_pair(&buf->pairs[buf->head], start_x, top, bot);
	buf->head = (buf->head + 1) % MAX_PIPES;
	buf->count++;
	return true;
}

bool pipebuf_despawn(Pipe_buffer* buf){
	
	if(buf->count == 0) return false;

	buf->pairs[buf->tail].exists = false;
	buf->tail = (buf->tail + 1) % MAX_PIPES;
	buf->count--;
	return true;
}

float pipebuf_getRightmost(const Pipe_buffer* buf){

	if(buf->count == 0) return GetScreenWidth();
	
	int last = (buf->head - 1 + MAX_PIPES) % MAX_PIPES;
	return buf->pairs[last].top.posX;

}

void pipebuf_update(Pipe_buffer* buf, float speed, float dt, Texture2D top, Texture2D bot){

	for(int i = 0; i < buf->count; i++){
		int idx = (buf->tail + i) % MAX_PIPES;
		Pipe_pair* pair = &buf->pairs[idx];

		pair->top.posX -= speed * dt;
		pair->bot.posX -= speed * dt;
		pair->hitbox.x -= speed * dt;
	}

	if(buf->count > 0){
		Pipe_pair* oldest = &buf->pairs[buf->tail];

		if(oldest->top.posX + PIPE_WIDTH < 0){
			pipebuf_despawn(buf);
			float spawn_x = pipebuf_getRightmost(buf) + PIPE_SPACING;
			pipebuf_spawn(buf, spawn_x, top, bot);
		}
	}
}

void pipebuf_render(Pipe_buffer* buf){

	for(int i = 0; i < buf->count; i++){
		int idx = (buf->tail + i) % MAX_PIPES;
		Pipe_pair* p = &buf->pairs[idx];
		if(!p->exists) continue;

		DrawTexture(p->top.sprite, p->top.posX, 
				p->top.posY, p->top.tint);
		DrawTexture(p->bot.sprite, p->bot.posX, 
				p->bot.posY, p->bot.tint);
	}

}

Rectangle get_pipe_hitbox(Pipe* p){
	return (Rectangle) {
		p->posX,
		p->posY,
		(float) PIPE_WIDTH,
		(float) PIPE_HEIGHT
	};
}

bool check_pipe_collision(Pipe_pair* p, Rectangle bird_hitbox){

	if(!p->exists) return false;

	Rectangle top_hitbox = get_pipe_hitbox(&p->top);
	Rectangle bot_hitbox = get_pipe_hitbox(&p->bot);

	return CheckCollisionRecs(bird_hitbox, top_hitbox) ||
	       CheckCollisionRecs(bird_hitbox, bot_hitbox);
}

bool pipebuf_check_collision(Pipe_buffer* buf, Rectangle bird_hitbox){

	for(int i = 0; i < buf->count; i++){
		int idx = (buf->tail + i) % MAX_PIPES;
		Pipe_pair* p = &buf->pairs[idx];
		if(check_pipe_collision(p, bird_hitbox))
			return true;
	}
	return false;
}

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

bool check_env_collision(Window* window, Bird* bird){
	return bird->posY > window->h || bird->posY < 0;
}

void game_reset(Window* window, Bird* bird, Pipe_buffer* buf, 
		Texture2D bird_tex, Texture2D top, Texture2D bot){
	window->score = 0;
	*bird = DEFAULT_BIRD;
	bird->sprite = bird_tex;
	pipebuf_init(buf);

	float start_x = GetScreenWidth() + 100.0f;
	for(int i = 0; i < MAX_PIPES; i++){
		pipebuf_spawn(buf, start_x + i * PIPE_SPACING,
				top, bot);
	}
}

void poll_events(Bird* bird, bool* show_hitboxes, bool* show_debug){

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


	Texture2D top_tex = LoadTexture("assets/pipe_top.png");
	Texture2D bot_tex = LoadTexture("assets/pipe_bot.png");
	Texture2D bird_tex = LoadTexture("assets/birb3.png");

	bool running = true;
	bool show_hitboxes = false;
	bool show_debug = false;
	

	Pipe_buffer pipe_buf;
	pipebuf_init(&pipe_buf);
	float pipe_speed = INITIAL_PIPE_SPEED;
	float start_x = GetScreenWidth() + 100.0f;
	const int dt = 1;
	for(int i = 0; i < MAX_PIPES; i++){
		pipebuf_spawn(&pipe_buf, start_x + i * PIPE_SPACING, top_tex, bot_tex);
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


		/* Main Loop */
		if(running){

		pipebuf_update(&pipe_buf, pipe_speed, dt, top_tex, bot_tex);
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
		if(show_debug){
			char hitbox_status[32];
			sprintf(hitbox_status, "%s", show_hitboxes ? "Hitboxes: Visible" :
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
		} else {
			DrawText("Press D to toggle debug info",
				 window->w - 900 , window->h - 50, 20, BLUE);
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
				window->attempts++;

				/* Reset game state */
				game_reset(window, bird, &pipe_buf,
						bird_tex, top_tex, bot_tex);
				
				/* we go agane */
				running = true;
			}
		}	

	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	//UnloadTexture(pair.top.sprite);
	//UnloadTexture(pair.bot.sprite);
	CloseWindow();
}
