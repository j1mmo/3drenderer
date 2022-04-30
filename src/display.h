#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL.h>
#include <assert.h>
#include <math.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

enum
{
	CULL_NONE,
	CULL_BACKFACE
} cull_method;

enum
{
	RENDER_WIRE,
	RENDER_WIRE_VERTEX,
	RENDER_FILL_TRIANGLE,
	RENDER_FILL_TRIANGLE_WIRE
} render_method;

extern SDL_Window* window;
extern int window_width;
extern int window_height;

extern SDL_Renderer* renderer;

extern SDL_Texture* color_buffer_texture;
extern uint32_t* color_buffer;


bool init_window(void);
void draw_grid(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_rect(int x, int y,int height,int width,uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_line(int x0, int y0, int x1, int y2, uint32_t color);
void render_color_buffer();
void clear_color_buffer(uint32_t color);
void cleanup(void);

#endif // DISPLAY_H_
