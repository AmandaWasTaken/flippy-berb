#include <raylib.h>
#include <stdbool.h>

typedef struct {
	float width;
	float height;
	Vector2 pos;
	Texture2D sprite;
	Rectangle bounds;
} Button;

void _draw_button_shadow(Button* b);
bool button_pressed(Button* b);
void draw_button(Button* b);
