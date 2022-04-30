#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "mesh.h"

vec3f_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1 }, // 1
    {.x = -1, .y = 1, .z = -1 }, // 2
    {.x = 1, .y = 1, .z = -1 }, // 3
    {.x = 1, .y = -1, .z = -1 }, // 4
    {.x = 1, .y = 1, .z = 1 }, // 5
    {.x = 1, .y = -1, .z = 1 }, // 6
    {.x = -1, .y = 1, .z = 1 }, // 7
    {.x = -1, .y = -1, .z = 1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3 },
    {.a = 1, .b = 3, .c = 4 },
    // right
    {.a = 4, .b = 3, .c = 5 },
    {.a = 4, .b = 5, .c = 6 },
    // back
    {.a = 6, .b = 5, .c = 7 },
    {.a = 6, .b = 7, .c = 8 },
    // left
    {.a = 8, .b = 7, .c = 2 },
    {.a = 8, .b = 2, .c = 1 },
    // top
    {.a = 2, .b = 7, .c = 5 },
    {.a = 2, .b = 5, .c = 3 },
    // bottom
    {.a = 6, .b = 8, .c = 1 },
    {.a = 6, .b = 1, .c = 4 }
};

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0},
    .scale = {1.0,1.0,1.0},
    .translation = {0,0,0}
};

void load_cube_mesh_data(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; ++i)
    {
        vec3f_t cube_vertex = cube_vertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    for (int i = 0; i < N_CUBE_FACES; ++i)
    {
        face_t cube_face = cube_faces[i];
        array_push(mesh.faces, cube_face);
    }
}

void load_obj_mesh_data(const char* filename)
{
    FILE* fp;
    fopen_s(&fp, filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return ;
    }

    #define MAX_LENGTH 256
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp))
    {
        const char first = buffer[0],
            second = buffer[1];
        if (first == 'v' && second == ' ')
        {
            vec3f_t vertice = { 0 };
            char* start = &buffer[2];
            char* end;
            vertice.x = strtof(start, &end);
            start = end;
            vertice.y = strtof(start, &end);
            start = end;
            vertice.z = strtof(start, &end);
            array_push(mesh.vertices, vertice);
        }
        else if (first == 'f' && second == ' ')
        {
            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];

            char* start = &buffer[2];
            char* end;
            for (int i = 0; i < 3; ++i)
            {
                vertex_indices[i] = strtol(start, &end, 10);
                start = end;
                start++;
                texture_indices[i] = strtol(start, &end, 10);
                start = end;
                start++;
                normal_indices[i] = strtol(start, &end, 10);
                start = end;
                start++;
            }
            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2],
                .color = 0xFF00FF00,
            };

            array_push(mesh.faces, face);
        }
    }
    vec3f_t n = { 0,0,0 };
    vec3f_t one = { 1,1,1 };
    vec3f_t translate = { 0,0,5 };
    mesh.rotation = n;
    mesh.scale = one;
    mesh.translation = translate;
}

