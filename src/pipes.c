#include <raylib.h>
#include "../lib/game.h"

#define INITIAL_PIPE_SPEED 2.75f
#define FONT_SIZE_GENERAL  22
#define FONT_SIZE_GAMEOVER 40
#define PIPE_SPACING 	   500.0f


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

bool pipebuf_append(Pipe_buffer* buf, float start_x, Texture2D top, Texture2D bot){
	
	if(buf->count == MAX_PIPES) return false;

	spawn_pipe_pair(&buf->pairs[buf->head], start_x, top, bot);
	buf->head = (buf->head + 1) % MAX_PIPES;
	buf->count++;
	return true;
}

bool pipebuf_remove(Pipe_buffer* buf){
	
	if(buf->count == 0) return false;

	buf->pairs[buf->tail].exists = false;
	buf->tail = (buf->tail + 1) % MAX_PIPES;
	buf->count--;
	return true;
}

float _pipebuf_getRightmost(const Pipe_buffer* buf){

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
			pipebuf_remove(buf);
			float spawn_x = _pipebuf_getRightmost(buf) + PIPE_SPACING;
			pipebuf_append(buf, spawn_x, top, bot);
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

Rectangle _get_pipe_hitbox(Pipe* p){
	return (Rectangle) {
		p->posX,
		p->posY,
		(float) PIPE_WIDTH,
		(float) PIPE_HEIGHT
	};
}

bool check_pipe_collision(Pipe_pair* p, Rectangle bird_hitbox){

	if(!p->exists) return false;

	Rectangle top_hitbox = _get_pipe_hitbox(&p->top);
	Rectangle bot_hitbox = _get_pipe_hitbox(&p->bot);

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
