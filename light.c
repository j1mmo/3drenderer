#include "light.h"

light_t light_ray = {
	.direction = { 0,0,1 },
};

color_t light_intensity(color_t og_color, float percentage_factor)
{
	if (percentage_factor < 0.05)
	{
		percentage_factor = 0.05;
	}
	if (percentage_factor > 0.95)
	{
		percentage_factor = 0.95;
	}
	uint32_t a = (og_color & 0xFF000000);
	uint32_t r = (og_color & 0x00FF0000) * percentage_factor;
	uint32_t b = (og_color & 0x0000FF00) * percentage_factor;
	uint32_t g = (og_color & 0x000000FF) * percentage_factor;

	uint32_t new_color = a | (r & 0x00FF0000) | (b & 0x0000FF00) | (g & 0x000000FF);

	return new_color;
}