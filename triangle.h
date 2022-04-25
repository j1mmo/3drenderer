#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdint.h>

#include "math.h"

typedef uint32_t color_t;

typedef struct
{
	int a;
	int b;
	int c;
	color_t color;
}face_t;

typedef struct
{
	vec2f_t points[3];
	color_t color;
	float avg_depth;
}triangle_t;


void print(triangle_t* t);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
int  partition(triangle_t* t, int p, int r);
void triangle_quicksort(triangle_t* t, int p, int r);
void depth_sort(triangle_t* triangles);
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int m_x, int m_y, uint32_t color);
void fill_flat_top_triangle(int x1, int y1, int m_x, int m_y, int x2, int y2, uint32_t color);

#endif // !TRIANGLE_H
