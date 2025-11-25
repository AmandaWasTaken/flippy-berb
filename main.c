#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

typedef struct {
	Texture2D sprite;
	float posX;
	float posY;
	float gravity;
	float jump_height;
	Color tint;
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
} Pipe_pair;

typedef struct {
	char* title;
	int w;
	int h;
} Window;

Pipe_pair spawn_pipe_pair(Window* window){

	const int gap_size = 200;
	const int error_margin = 20;

	Texture2D top_tex = LoadTexture("assets/pipe_top.png");
	Texture2D bot_tex = LoadTexture("assets/pipe_bot.png");

	int min_gap = error_margin;
	int max_gap = window->h - gap_size - error_margin;

	int gap_y = GetRandomValue(min_gap, max_gap);

	Pipe top = {
		.sprite = top_tex, 
		.posX = window->w,
		.posY = gap_y - top.sprite.height, 
		.tint = WHITE,
	};

	Pipe bot = {
		.sprite = bot_tex, 
		.posX = window->w,
		.posY = gap_y + gap_size, 
		.tint = WHITE,
	};

	Pipe_pair ret = {
		.top = top,
		.bot = bot,
		.exists = true,
	};

	return ret;
}

void poll_events(Bird* bird){

	if(IsKeyPressed(KEY_SPACE)){
		fprintf(stdout, "Space pressed\n");
		bird->posY -= bird->jump_height;
		bird->posY -= (bird->jump_height) - 2.5f;
		bird->posY -= bird->jump_height   - 1.25f;
	}	
} 

void event_loop(Bird* bird, Window* window){

	Pipe_pair pair = {0};
	pair.exists = false;

	while(!WindowShouldClose()){
		
	BeginDrawing();

		int should_spawn_pipes = rand() % 13;	

		if(should_spawn_pipes == 0 && !pair.exists){
			pair = spawn_pipe_pair(window);
			fprintf(stdout, "Pair Spawned\n");
		}

		ClearBackground(RAYWHITE);
		DrawFPS(10, 10);

		poll_events(bird);

		// shits backwards; bird falls down with +=
		bird->posY += bird->gravity;

		DrawTexture(bird->sprite,
			    bird->posX,
			    bird->posY,
			    bird->tint);

		if(pair.exists){
			const int pipe_mvmt = 4; // movement speed of pipes
			pair.top.posX -= pipe_mvmt;
			DrawTexture(pair.top.sprite,
		    		pair.top.posX,
    		    		pair.top.posY,
			    	pair.top.tint);

			pair.bot.posX -= pipe_mvmt;
			DrawTexture(pair.bot.sprite,
		   		pair.bot.posX,
			   	pair.bot.posY,
		   		pair.bot.tint);

			// Ready to spawn a new pair of pipes
			// (Previous pair has despawned)
			if(pair.top.posX + pair.top.sprite.width < 0){
				pair.exists = false;
				fprintf(stdout, "Pair Despawned\n");
				pair.exists = false;
			}
		}

	EndDrawing();
	}

	UnloadTexture(bird->sprite);
	CloseWindow();
}


int main(int argc, char** argv){

	// For "random" pipe spawning 
	srand(time(NULL));

	Window game = {
		.title = "Birb",
		.w = 1000,
		.h = 700,
	};

	InitWindow(game.w, game.h, game.title);
	SetTargetFPS(60);

	Bird bird = {
		.sprite = LoadTexture("assets/birb3.png"),
		.posX = 300,
		.posY = 300,
		.gravity = 5.0f,
		.jump_height = 32.25f,
		.tint = WHITE
	};

	event_loop(&bird, &game);
        return 0;
}
