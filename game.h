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
	bool counted;
} Pipe_pair;

typedef struct {
	char* title;
	int w;
	int h;
	int score;
} Window;

Pipe_pair spawn_pipe_pair(Window* window);
void poll_events(Bird* bird, bool* show_hitboxes);
void event_loop(Bird* bird, Window* window);
bool check_env_collision(Window* window, Bird* bird);
void game_over(Window* window);

