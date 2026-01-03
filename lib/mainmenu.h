#include <raylib.h>
#include <stdbool.h>
#include "../lib/button.h"
#include "../lib/game.h"

void render_menu(Window* w, bool* running,
		bool* first_launch,
		Texture2D b_start_sprite,
		Texture2D b_quit_sprite,
		Texture2D b_scores_sprite,
		Texture2D menu_bg);
