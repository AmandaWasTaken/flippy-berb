#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>

/* Clickable button */
typedef struct {
	float width;		// Button width
	float height;		// Button height
	Vector2 pos;		// Button position {x, y}
	Texture2D sprite;	// Button image
	Rectangle bounds;	// Button hitbox boundaries
} Button;


enum Button_state {
	NORMAL,
	HOVER,
	PRESSED
};

/* Draw a box shadow next to button (top left only for now) */
void _draw_button_shadow(Button* b){
	DrawRectangle(b->pos.x - 2.0f, b->pos.y - 2.0f, b->width,
			b->height, DARKGREEN);
}

/* Check if button was clicked with mouse1 */
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

/* Render a button object */
void draw_button(Button* b){
	DrawTexture(b->sprite, b->pos.x, b->pos.y, WHITE);
}
