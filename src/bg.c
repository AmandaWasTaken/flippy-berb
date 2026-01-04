#include <raylib.h>
#include "../lib/game.h"

/* Render game background */
void render_background(Window* w, Texture2D bg_texture, float* bg_offset){

	const int bg_width = w->w + 150;
	if(*bg_offset >= bg_width) *bg_offset = 0.0f;
	// Render bg image to the left and right to make it loop seamlessly
	DrawTexture(bg_texture, (int)(-(*bg_offset)), 0, WHITE);
	DrawTexture(bg_texture, (int)(bg_width - (*bg_offset)), 0, WHITE);
}
