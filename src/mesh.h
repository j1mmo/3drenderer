#ifndef MESH_H
#define MESH_H

#include "math.h"
#include "triangle.h"


#define N_CUBE_VERTICES 8
extern vec3f_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2)
extern face_t cube_faces[N_CUBE_FACES];

typedef struct
{
	vec3f_t * vertices;
	face_t  * faces;
	vec3f_t   rotation;
	vec3f_t   scale;
	vec3f_t   translation;
}mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);
void load_obj_mesh_data(const char * filename);

#endif // MESH_H


