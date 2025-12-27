#define INITIAL_PIPE_SPEED 2.75f
#define FONT_SIZE_GENERAL  22
#define FONT_SIZE_GAMEOVER 40
#define PIPE_SPACING 	   500.0f

/* Initialize buffer */
void pipebuf_init(Pipe_buffer* buf);

/* Spawn next pipe from buffer */
void spawn_pipe_pair(Pipe_pair* pair, float start_x, Texture2D top, Texture2D bot);

/* was pipebuf_spawn, appends new pair to buffer */
bool pipebuf_append(Pipe_buffer* buf, float start_x, Texture2D top, Texture2D bot);

/* was pipebuf_despawn, removes pair from buffer */
bool pipebuf_remove(Pipe_buffer* buf);

/* Helper function returns x position of the rightmost pair in buffer */
float _pipebuf_getRightmost(const Pipe_buffer* buf);

/* Update all pair positions in buffer */
void pipebuf_update(Pipe_buffer* buf, float speed, float dt, Texture2D top, Texture2D bot);

/* Render pipes currently in buffer */
void pipebuf_render(Pipe_buffer* buf);

/* Helper function for fetching a pipe's hitbox */
Rectangle _get_pipe_hitbox(Pipe* p);

/* Functions for checking collisions with pipes */
bool check_pipe_collision(Pipe_pair* p, Rectangle bird_hitbox);
bool pipebuf_check_collision(Pipe_buffer* buf, Rectangle bird_hitbox);
