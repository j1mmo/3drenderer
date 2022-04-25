#include "display.h"
#include "math.h"
#include "light.h"
#include "mesh.h"
#include "array.h"

void setup(void);
void process_input(void);
void update(void);
void render(void);
void free_resources(void);


bool running = false;

int previous_frame_time = 0;

mat4_t projection_matrix;

vec3f_t camera_position = { .x = 0, .y = 0, .z = -5 };

triangle_t* triangles_to_render;

int main(int argc, char** args) 
{
	running = init_window();
	setup();

	while (running) 
	{

		int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
		if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
		{
			SDL_Delay(time_to_wait);
		}
		

		previous_frame_time = SDL_GetTicks();

		process_input();

		update();

		render();

	}
	cleanup();
	free_resources();
	return 0;
}

void free_resources(void)
{
	free(color_buffer);

	array_free(mesh.faces);
	array_free(mesh.vertices);
}

void setup(void) 
{
	render_method = RENDER_WIRE;
	cull_method = CULL_BACKFACE;

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
	    renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	float fov = degrees_to_radians(60);
	float aspect_ratio = (float)window_height / (float)window_width;
	const float znear = 0.1;
	const float zfar = 100.0;
	projection_matrix = mat4_make_perspective(fov, aspect_ratio, znear, zfar);
	
	load_obj_mesh_data(".\\assets\\f22.obj");
}


void process_input(void) 
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) 
	{
		case SDL_QUIT: {
			running = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
				break;
			}
			if (event.key.keysym.sym == SDLK_0)
			{
				render_method = RENDER_WIRE_VERTEX;
			}
			if (event.key.keysym.sym == SDLK_1)
			{
				render_method = RENDER_WIRE;
			}
			if (event.key.keysym.sym == SDLK_2)
			{
				render_method = RENDER_FILL_TRIANGLE;
			}
			if (event.key.keysym.sym == SDLK_3)
			{
				render_method = RENDER_FILL_TRIANGLE_WIRE;
			}
			if (event.key.keysym.sym == SDLK_c)
			{
				cull_method = CULL_BACKFACE;
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				cull_method = CULL_NONE;
			}
		}
	}
}

void update(void) 
{
	triangles_to_render = NULL;

	mesh.rotation.x += 0.01;
	mesh.rotation.y -= 0.01;
	//mesh.rotation.z += 0.01;

	//mesh.scale.x += 0.002;

	mat4_t scale_m = mat4_init_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t trans_m = mat4_init_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotate_m_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotate_m_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotate_m_z = mat4_make_rotation_z(mesh.rotation.z);

	int num_faces = array_length(mesh.faces);
	for (int i = 0; i < num_faces; i++) {
		face_t mesh_face = mesh.faces[i];

		vec3f_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

		

		vec4f_t transformed_vertices[3] = { 0 };
		
		// Loop all three vertices of this current face and apply transformations
		for (int j = 0; j < 3; j++) {
			vec4f_t transformed_vertex = {
				.x = face_vertices[j].x,
				.y = face_vertices[j].y,
				.z = face_vertices[j].z,
				.w = 1
			};

			mat4_t world_matrix = mat4_id();
			world_matrix = mat4_mult_mat4(rotate_m_z, world_matrix);
			world_matrix = mat4_mult_mat4(rotate_m_y, world_matrix);
			world_matrix = mat4_mult_mat4(rotate_m_x, world_matrix);
			//world_matrix = mat4_mult_mat4(rotate_m_x, world_matrix);
			//world_matrix = mat4_mult_mat4(rotate_m_y, world_matrix);
			//
			//world_matrix = mat4_mult_mat4(trans_m, world_matrix);

			world_matrix = mat4_mult_mat4(trans_m, world_matrix);
	
			transformed_vertex = mat4_mult_vec4(world_matrix, transformed_vertex);
			// Translate the vertex away from the camera
			transformed_vertex.z += 5;

			transformed_vertices[j] = transformed_vertex;
		}

		

		//giving meaningful names
		vec3f_t vector_a = vec3_from_vec4(transformed_vertices[0]);
		vec3f_t vector_b = vec3_from_vec4(transformed_vertices[1]);
		vec3f_t vector_c = vec3_from_vec4(transformed_vertices[2]);

		vec3f_t vector_ab = vec3f_sub(vector_b, vector_a);
		vec3f_normalise(&vector_ab);

		vec3f_t vector_ac = vec3f_sub(vector_c, vector_a);
		vec3f_normalise(&vector_ac);

		vec3f_t normal = vec3f_cross(vector_ab, vector_ac);
		vec3f_normalise(&normal);

		vec3f_t camera_ray = vec3f_sub(camera_position, vector_a);

		float dot_normal_camera = vec3f_dot(normal, camera_ray);

		if (cull_method == CULL_BACKFACE) {	//back culling
			if (dot_normal_camera < 0)
			{
				continue;
			}
		}

		

		vec4f_t projected_points[3] = { 0 };

		for(int j = 0; j < 3; ++j) {
			// Project the current vertex
			projected_points[j] = mat4_mult_vec4_project(projection_matrix, transformed_vertices[j]);
			
			// translate the projected points to the middle of the screen

			projected_points[j].x *= (window_width / 2.0);
			projected_points[j].y *= (window_width / 2.0);

			projected_points[j].y *= -1;

			projected_points[j].x += (window_width / 2.0);
			projected_points[j].y += (window_height / 2.0);
			
		}

		float depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

		
		float light_intensity_factor = -vec3f_dot(normal, light_ray.direction);
		uint32_t triangle_color = light_intensity(mesh_face.color, light_intensity_factor);

		triangle_t projected_triangle = {
			.points = {
				{ projected_points[0].x, projected_points[0].y },
				{ projected_points[1].x, projected_points[1].y },
				{ projected_points[2].x, projected_points[2].y },
			},
			.color = triangle_color,
			.avg_depth = depth,
		};
		
		// Save the projected triangle in the array of triangles to render
		array_push(triangles_to_render, projected_triangle);
	}

	depth_sort(triangles_to_render);
}

void render(void) 
{
	
	int number_of_triangles = array_length(triangles_to_render);
	for (int i = 0; i < number_of_triangles; ++i)
	{
		triangle_t triangle = triangles_to_render[i];
		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y, // vertex A
				triangle.points[1].x, triangle.points[1].y, // vertex B
				triangle.points[2].x, triangle.points[2].y, // vertex C
				triangle.color
			);
		}

		// Draw triangle wireframe
		if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE) {
			draw_triangle(
				triangle.points[0].x, triangle.points[0].y, // vertex A
				triangle.points[1].x, triangle.points[1].y, // vertex B
				triangle.points[2].x, triangle.points[2].y, // vertex C
				0xFFFFFFFF
			);
		}

		// Draw triangle vertex points
		if (render_method == RENDER_WIRE_VERTEX) {
			draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFF0000); // vertex A
			draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFF0000); // vertex B
			draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFF0000); // vertex C
		}
	}
	
	//draw_triangle(300, 100, 50, 400, 500, 700, 0xFFFFFF00);
	//draw_filled_triangle(300, 100, 50, 400, 500, 700, 0xFFFFFF00);


	array_free(triangles_to_render);
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}


