#ifndef LIGHT_H
#define LIGHT_H
#include <stdint.h>
#include "math.h"

typedef struct
{
	vec3f_t direction;
}light_t;

typedef uint32_t color_t;

extern light_t light_ray;

color_t light_intensity(color_t og_color, float percentage_factor);

#endif //LIGHT_T