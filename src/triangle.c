#include <stdio.h>
#include "display.h"
#include "array.h"

#include "triangle.h"

void print(triangle_t* t)
{
	printf("points 0: %f %f\npoints 1: %f %f\npoints 2: %f %f\n\n\n", t->points[0].x, t->points[0].y, t->points[1].x, t->points[1].y, t->points[2].x, t->points[2].y);
}

void triangle_swap(triangle_t* a, triangle_t* b)
{
	triangle_t temp = *a;
	*a = *b;
	*b = temp;
}

int partition(triangle_t* t, int p, int r)
{
	triangle_t x = t[r];
	int i = p - 1;
	for (int j = p; j < r - 1; ++j)
	{
		if (t[j].avg_depth > x.avg_depth)
		{
			i += 1;
			triangle_swap(&t[i], &t[j]);
		}
	}
	triangle_swap(&t[i + 1], &t[r]);
	return i + 1;
}

void triangle_quicksort(triangle_t* t, int p, int r)
{
	if (p < r)
	{
		int q = partition(t, p, r);
		triangle_quicksort(t, p, q - 1);
		triangle_quicksort(t, q + 1, r);
	}
}

void depth_sort(triangle_t* triangles)
{
	int length = array_length(triangles);
	triangle_quicksort(triangles, 0, length-1);
}

void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void fill_flat_bottom_triangle(int x0,int y0,int x1,int y1,int x2,int y2, uint32_t color)
{

	float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

	float x_start = x0;
	float x_end = x0;

	for (int y = y0; y <= y2; ++y)
	{
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
}

void fill_flat_top_triangle(int x0,int y0,int x1,int y1,int x2,int y2, uint32_t color)
{
	float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
	float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

	float x_start = x2;
	float x_end = x2;

	for (int y = y2; y >= y0; --y)
	{
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	if (y0 > y1) 
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2)
	{
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) 
	{
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}


	if (y1 == y2) 
	{
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1)
	{
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else
	{
		int m_y = y1;
		int m_x = ((float)(x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0;

		fill_flat_bottom_triangle(x0, y0, x1, y1, m_x, m_y, color);
		fill_flat_top_triangle(x1, y1, m_x, m_y, x2, y2, color);
	}
}