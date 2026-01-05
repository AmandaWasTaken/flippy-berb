#include <raylib.h>

#include "../lib/game.h"
#include "../lib/scores.h"

void render_deathscreen(Window* w, bool* running,
			Texture2D b_retry_sprite,
			Texture2D b_score_reset_sprite,
			Texture2D b_quit_sprite,
			Texture2D death_bg);
