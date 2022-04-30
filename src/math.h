#ifndef MATH_H
#define MATH_H

#define PI 3.14159

typedef struct
{
	float x, y;
}vec2f_t;

typedef struct
{
	float x, y, z;
}vec3f_t;

typedef struct
{
	float x, y, z, w;
}vec4f_t;

typedef struct
{
	float m[4][4];
}mat4_t;

/*
* matrix 4x4 functions
*/

mat4_t mat4_id();
mat4_t mat4_init_scale(float sx, float sy, float sz);
mat4_t mat4_init_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);
mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar);
vec4f_t mat4_mult_vec4_project(mat4_t mat_projection, vec4f_t v);

mat4_t mat4_mult_mat4(mat4_t m, mat4_t n);
vec4f_t mat4_mult_vec4(mat4_t m, vec4f_t v);

float radians_to_degrees(float radians);
float degrees_to_radians(float degrees);
/*
* Vector 2 functions
*/
float vec2f_length(vec2f_t v);

vec2f_t vec2f_add(vec2f_t a, vec2f_t b);
vec2f_t vec2f_sub(vec2f_t a, vec2f_t b);
vec2f_t vec2f_mul(vec2f_t a,float scalar);
vec2f_t vec2f_div(vec2f_t a, float scalar);
float   vec2f_dot(vec2f_t a, vec2f_t b);
void vec2f_normalise(vec2f_t* normal);

/*
* Vector 3 functions
*/
float vec3f_length(vec3f_t v);
vec3f_t vec3_from_vec4(vec4f_t v);
vec3f_t vec3f_add(vec3f_t a, vec3f_t b);
vec3f_t vec3f_sub(vec3f_t a, vec3f_t b);
vec3f_t vec3f_mul(vec3f_t a, float scalar);
vec3f_t vec3f_div(vec3f_t a, float scalar);


vec3f_t vec3f_cross(vec3f_t a, vec3f_t b);
float   vec3f_dot(vec3f_t a, vec3f_t b);
void    vec3f_normalise(vec3f_t* normal);
vec3f_t vec3_rotate_x(vec3f_t v, float angle);
vec3f_t vec3_rotate_y(vec3f_t v, float angle);
vec3f_t vec3_rotate_z(vec3f_t v, float angle);

void print_vec3f(vec3f_t* v);

/*
* Vector 4 functions
*/

vec4f_t vec4_from_vec3(vec3f_t v);

#endif //MATH_H