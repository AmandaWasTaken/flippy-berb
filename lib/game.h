#ifndef GAME_H
#define GAME_H

#define MAX_PIPES 3
#define PIPE_HEIGHT 600
#define PIPE_WIDTH 200

#include <raylib.h>

typedef struct {
	Texture2D sprite;
	float posX;
	float posY;
	float velocityY;
	float gravity;
	float jump_velocity;
	Color tint;
	Rectangle hitbox;
} Bird;

extern Bird DEFAULT_BIRD;

typedef struct {
	Texture2D sprite;
	float posX;
	float posY;
	Color tint;
} Pipe;

typedef struct {
	Pipe top;
	Pipe bot;
	bool exists;
	Rectangle hitbox;
	bool counted;    // false if pair has not been passed
} Pipe_pair;

typedef struct {
	Pipe_pair pairs[MAX_PIPES];
	int head;	// Position of next pair
	int tail;	// Leftmost pair
	int count;
} Pipe_buffer;

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
void poll_events(Bird* bird, bool* show_hitboxes, bool* show_debug);
void event_loop(Bird* bird, Window* window);
bool check_env_collision(Window* window, Bird* bird);
void game_over(Window* window);

#endif
