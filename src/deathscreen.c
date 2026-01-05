#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "../lib/game.h"
#include "../lib/button.h"
#include "../lib/scores.h"

void render_deathscreen(Window* w, bool* running,
			Texture2D b_retry_sprite,
			Texture2D b_score_reset_sprite,
			Texture2D b_quit_sprite,
			Texture2D death_bg){

	const int   button_w = 300;
	const int   button_h = 75;
	const float button_posX = w->w/2 - button_w/2; // Center buttons horizontally
	const float button_spacing_vert = (float)(w->h)/3 - 50; // Vertical gap 
	
	Button retry = {
		.pos.x  = button_posX,
		.pos.y  = w->h - button_h - 500,
		.width  = button_w,
		.height = button_h
	};

	retry.sprite = b_retry_sprite;
	retry.bounds = (Rectangle){
		retry.pos.x,
		retry.pos.y,
		(float)button_w,
		(float)button_h,
	};

	Button score_reset = {
		.pos.x = button_posX,
		.pos.y = retry.pos.y + button_spacing_vert,
		.width = button_w,
		.height = button_h,
	};

	score_reset.sprite = b_score_reset_sprite;
	score_reset.bounds = (Rectangle){
		score_reset.pos.x,
		score_reset.pos.y,
		(float)button_w,
		(float)button_h,
	};

	Button quit = {
		.pos.x  = button_posX,
		.pos.y  = score_reset.pos.y + button_spacing_vert,
		.width  = button_w,
		.height = button_h
	};
	quit.sprite = b_quit_sprite;
	quit.bounds = (Rectangle){
		quit.pos.x,
		quit.pos.y,
		(float)button_w,
		(float)button_h,
	};

	if(button_pressed(&retry)){
		w->attempts++;
		*running = true;
		return;
	}
	
	if(button_pressed(&score_reset)){
		fprintf(stdout, "Erasing scores. . .\n\n");
		reset_scores();
	}

	if(button_pressed(&quit)){
		exit(0);
	}

	DrawTexture(death_bg, 0, 0, WHITE);
	draw_button(&retry);
	draw_button(&score_reset);
	draw_button(&quit);

	return;
}

