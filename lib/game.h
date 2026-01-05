#ifndef GAME_H
#define GAME_H

#define MAX_PIPES 3
#define PIPE_HEIGHT 600
#define PIPE_WIDTH 200

#include <raylib.h>
#include "game.h"

// Player character (bird)
typedef struct {
	Texture2D sprite;	// Bird texture
	float posX;		// X-position
	float posY;		// Y-position
	float velocityY;	// Vertical velocity
	float gravity;		// Gravity strenght
	float jump_velocity;	// How high bird should jump
	Color tint;		
	Rectangle hitbox;
} Bird;

extern Bird DEFAULT_BIRD;	// Default settings for
				// a Bird object

// A single pipe for pipe pairs
typedef struct {
	Texture2D sprite;	// Pipe texture
	float posX;		// X-position
	float posY;		// Y-position
	Color tint;
} Pipe;

// Pair consisting of top and bottom pipes
typedef struct {
	Pipe top;
	Pipe bot;
	bool exists;	 // Ignore pipe in rendering and stuff
			 // if it doesn't exist anymore
	Rectangle hitbox;
	bool counted;    // false if pair has not been passed
} Pipe_pair;

// Ring buffer sort of deal 
typedef struct {
	Pipe_pair pairs[MAX_PIPES];
	int head;	// Position of next pair
	int tail;	// Leftmost pair
	int count;
} Pipe_buffer;

// Game window
typedef struct {
	char* title;
	int w;
	int h;
	int score;
	int attempts;
} Window;

void update_score(Pipe_buffer* buf, Bird* bird, int* score, float* speed);
void render_score(Window* window);
void render_attempt(Window* window);
void render_leaderboard(Window* window);

// Pipe_pair spawn_pipe_pair(Window* window);
void poll_events(Bird* bird, bool* show_debug, bool* show_hitboxes);
void event_loop(Bird* bird, Window* window);
bool check_env_collision(Window* window, Bird* bird);
void game_reset(Window* window, Bird* bird, Pipe_buffer* buf,
		Texture2D bird_tex, Texture2D top, Texture2D bot);

void draw_debug_info(Window* window, Bird* bird, 
		bool* show_hitboxes, float pipe_speed);

void end_game(Window* window, bool* running, Bird* bird,
	      Texture2D top, Texture2D bot, Texture2D bird_sprite,
	      Texture2D b_retry_sprite, 
	      Texture2D b_score_reset_sprite,
	      Texture2D b_quit_sprite, 
	      Texture2D death_bg, Pipe_buffer* buf);

#endif
