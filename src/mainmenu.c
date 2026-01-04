#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <stdlib.h>
#include "../lib/game.h"
#include "../lib/button.h"


// Render main menu before 'start' is clicked
// @Param main window object
// @Param flag for game state (true = 'start' has been clicked)
// @Param flag to make sure menu is only rendered once and not every time player dies
// @Params menu button images and menu background image
void render_menu(Window* w, bool* running,
		bool* first_launch, 
		Texture2D b_start_sprite,
		Texture2D b_quit_sprite,
		Texture2D b_scores_sprite,
		Texture2D menu_bg){

	// All buttons have the same dimensions
	const int   button_w = 300;
	const int   button_h = 75;
	const float button_posX = w->w/2 - button_w/2; // Center buttons horizontally
	const float button_spacing_vert = (float)(w->h)/3 - 50; // Vertical gap 


	/* Make start button */
	Button start    = {
		.pos.x  = button_posX,
		.pos.y  = w->h - button_h - 500,
		.width  = button_w,
		.height = button_h,
	};

	start.sprite = b_start_sprite;
	start.bounds = (Rectangle){
		start.pos.x,
		start.pos.y,
		(float)button_w,
		(float)button_h,
	};

	/* Make lb button (currently unusued) */ 
	Button scores   = {
		.pos.x  = button_posX,
		.pos.y  = start.pos.y + button_spacing_vert,
		.width  = button_w,
		.height = button_h,
	};
	scores.sprite = b_scores_sprite;
	scores.bounds = (Rectangle){
		scores.pos.x,
		scores.pos.y,
		(float)button_w,
		(float)button_h,
	};

	/* Make quit button */
	Button quit     = {
		.pos.x  = button_posX,
		.pos.y  = scores.pos.y + button_spacing_vert,
		.width  = button_w,
		.height = button_h,
	};
	quit.sprite = b_quit_sprite;
	quit.bounds = (Rectangle){
		quit.pos.x,
		quit.pos.y,
		(float)button_w,
		(float)button_h,
	};


	/* Handle button actions */
	if(button_pressed(&start)) { 
		*running = true;
		*first_launch = false;
	}
	if(button_pressed(&scores)){
		; // unimplemented
	}
	if(button_pressed(&quit)){
		// TODO This is not very sexy
		exit(1);
	}

	/* Render buttons and bg */
	DrawTexture(menu_bg, 0, 0, WHITE);
	draw_button(&start);
	draw_button(&scores);
	draw_button(&quit);

	return;
}
