#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
	float width;
	float height;
	Vector2 pos;
	Texture2D sprite;
	Rectangle bounds;
} Button;

enum Button_state {
	NORMAL,
	HOVER,
	PRESSED
};

void _draw_button_shadow(Button* b){
	DrawRectangle(b->pos.x - 2.0f, b->pos.y - 2.0f, b->width,
			b->height, DARKGREEN);
}

bool button_pressed(Button* b){

	Vector2 mouse_pos = {0.0f, 0.0f};
	bool button_action = false;
	int state = NORMAL;

	mouse_pos = GetMousePosition();
	if(CheckCollisionPointRec(mouse_pos, b->bounds)){

		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			state = PRESSED;
		} else {
			_draw_button_shadow(b);
			state = HOVER;
			b->pos.x += 2.75f;
			b->pos.y += 2.75f;
			
		}
		if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
			button_action = true;
		}
	} else { state = NORMAL; }

	if(button_action){
		return true;
	}

	return false;
}

void draw_button(Button* b){
	DrawTexture(b->sprite, b->pos.x, b->pos.y, WHITE);
}
