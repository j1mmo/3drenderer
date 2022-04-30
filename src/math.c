#include <math.h>
#include "math.h"


mat4_t mat4_id()
{
	mat4_t m = {
		.m = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
		} 
	};
	return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar)
{
	mat4_t m = { {0} };
	m.m[0][0] = aspect * (1 / tanf(fov / 2));
	m.m[1][1] = 1 / tan(fov / 2);
	m.m[2][2] = zfar / (zfar - znear);
	m.m[3][3] = (-zfar * znear) / (zfar - znear);
	m.m[3][2] = 1.0;

	return m;
}

vec4f_t mat4_mult_vec4_project(mat4_t mat_projection, vec4f_t v)
{
	vec4f_t result = mat4_mult_vec4(mat_projection, v);

	if (result.w != 0.0f)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}
	return result;
}

vec4f_t mat4_mult_vec4(mat4_t m, vec4f_t v)
{
	vec4f_t result = { 0,0,0,0 };
	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
	return result;
}

mat4_t mat4_init_translation(float tx, float ty, float tz)
{
	mat4_t m = mat4_id();
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	return m;
}

mat4_t mat4_make_rotation_x(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t m = mat4_id();
	m.m[1][1] = c;
	m.m[1][2] = s;
	m.m[2][1] = -s;
	m.m[2][2] = c;

	return m;
}

mat4_t mat4_make_rotation_y(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t m = mat4_id();
	m.m[0][0] = c;
	m.m[0][2] = s;
	m.m[2][0] = -s;
	m.m[2][2] = c;

	return m;
}

mat4_t mat4_make_rotation_z(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	mat4_t m = mat4_id();
	m.m[0][0] = c;
	m.m[0][1] = -s;
	m.m[1][0] = s;
	m.m[1][1] = c;

	return m;
}

mat4_t mat4_init_scale(float sx, float sy, float sz)
{
	mat4_t id = mat4_id();
	id.m[0][0] = sx;
	id.m[1][1] = sy;
	id.m[2][2] = sz;
	return id;
}

mat4_t mat4_mult_mat4(mat4_t a, mat4_t b)
{
	mat4_t result = { 0 };
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			for (int traverse = 0; traverse < 4; ++traverse)
			{
				result.m[row][col] += a.m[row][traverse] * b.m[traverse][col];
			}
		}
	}
	return result;
}

/*
	general functions
*/
float radians_to_degrees(float radians)
{
	return radians * (180 / PI);
}

float degrees_to_radians(float degrees)
{
	return degrees * (PI / 180);
}
/*
* Vector 2d functions
*/
float vec2f_length(vec2f_t v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

vec2f_t vec2f_add(vec2f_t a, vec2f_t b)
{
	vec2f_t v = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return v;
}

vec2f_t vec2f_sub(vec2f_t a, vec2f_t b)
{
	vec2f_t v = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return v;
}

void vec2f_normalise(vec2f_t* normal)
{
	const float length = sqrtf(normal->x * normal->x + normal->y * normal->y);
	normal->x /= length;
	normal->y /= length;
}

vec2f_t vec2f_mul(vec2f_t a, float scalar)
{
	vec2f_t v = {
		.x = a.x * scalar,
		.y = a.y * scalar
	};
	return v;
}

vec2f_t vec2f_div(vec2f_t a, float scalar)
{
	vec2f_t v = {
		.x = a.x / scalar,
		.y = a.y / scalar
	};
	return v;
}

float vec2f_dot(vec2f_t a, vec2f_t b)
{
	return (a.x * b.x) + (a.y * b.y);
}

/*
Vector 3d functions
*/

vec3f_t vec3_from_vec4(vec4f_t v)
{
	vec3f_t r = {
		.x = v.x,
		.y = v.y,
		.z = v.z
	};
	return r;
}

float vec3f_length(vec3f_t v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3f_t vec3f_add(vec3f_t a, vec3f_t b)
{
	vec3f_t v = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return v;
}

vec3f_t vec3f_sub(vec3f_t a, vec3f_t b)
{
	vec3f_t v = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return v;
}

vec3f_t vec3f_mul(vec3f_t a, float scalar)
{
	vec3f_t v = {
		.x = a.x * scalar,
		.y = a.y * scalar,
		.z = a.z * scalar
	};
	return v;
}

vec3f_t vec3f_div(vec3f_t a, float scalar)
{
	vec3f_t v = {
		.x = a.x / scalar,
		.y = a.y / scalar,
		.z = a.z / scalar
	};
	return v;
}



vec3f_t vec3_rotate_x(vec3f_t v, float angle)
{
	vec3f_t rotate_vector = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
	return rotate_vector;
}

vec3f_t vec3_rotate_y(vec3f_t v, float angle)
{
	vec3f_t rotate_vector = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle),
	};
	return rotate_vector;
}

vec3f_t vec3_rotate_z(vec3f_t v, float angle)
{
	vec3f_t rotate_vector = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y	* cos(angle),
		.z = v.z
	};
	return rotate_vector;
}

vec3f_t vec3f_cross(vec3f_t a, vec3f_t b)
{
	vec3f_t n = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return n;
}

void vec3f_normalise(vec3f_t* normal)
{
	const float length = sqrtf(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
	normal->x /= length;
	normal->y /= length;
	normal->z /= length;
}

float vec3f_dot(vec3f_t a, vec3f_t b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void print_vec3f(vec3f_t* v)
{
	printf("%f %f %f\n", v->x, v->y, v->z);
}

/*
* Vector 4 functions
*/

vec4f_t vec4_from_vec3(vec3f_t v)
{
	vec4f_t r = {
		.x = v.x,
		.y = v.y,
		.z = v.z,
		.w = 1
	};
	return r;
}