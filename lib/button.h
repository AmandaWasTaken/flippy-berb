#include <raylib.h>
#include <stdbool.h>

// Reusable button type
typedef struct {
	float width;
	float height;
	Vector2 pos;		// Position {x, y}
	Texture2D sprite;	// Button image
	Rectangle bounds;	// Hitbox boundaries
} Button;

void _draw_button_shadow(Button* b);
bool button_pressed(Button* b);
void draw_button(Button* b);
