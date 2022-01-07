#include <iostream>
#include <SDL2/SDL.h>
#include <GLES3/gl3.h>
#include <SDL_image.h>
#include <cstdint>

#include "vange_rs.h"
#define HANDMADE_MATH_IMPLEMENTATION
#include "HandmadeMath.h"

using namespace std;

int main()
{
	const int window_width = 1280;
	const int window_height = 720;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

	int img_init_flags = IMG_INIT_PNG;
	if((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags){
		printf("error initializing SDL2_image: %s\n", IMG_GetError());
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_Window* window = SDL_CreateWindow("rv_ffi",
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   window_width,
									   window_height,
									   SDL_WINDOW_OPENGL);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	const GLubyte* gl_version = glGetString(GL_VERSION);
	std::cout << "GL version: " << gl_version << std::endl;

	const char* height_path = "/home/nikita/Sources/Test3DMap/indexed/export/height.png";
	SDL_Surface* height_surface = IMG_Load(height_path);
	if(height_surface == nullptr){
		printf("error loading height image \"%s\": %s\n", height_path, IMG_GetError());
	}

	const char* meta_path = "/home/nikita/Sources/Test3DMap/indexed/export/height.png";
	SDL_Surface* meta_surface = IMG_Load(meta_path);
	if(meta_surface == nullptr){
		printf("error loading meta image \"%s\": %s\n", meta_path, IMG_GetError());
	}

	const char* table_path = "/home/nikita/Sources/Test3DMap/indexed/export/table.png";
	SDL_Surface* table_surface = IMG_Load(table_path);
	if(table_surface == nullptr){
		printf("error loading table image \"%s\": %s\n", table_path, IMG_GetError());
	}

	uint8_t* height_bytes = (uint8_t*)height_surface->pixels;
	uint8_t* meta_bytes = (uint8_t*)meta_surface->pixels;

	int32_t map_width = height_surface->w;
	int32_t map_height = height_surface->h;

	uint8_t** lineT = new uint8_t*[map_height];
	for(int i = 0; i < map_height; i++){
		lineT[i] = new uint8_t[map_width * 2];
		std::copy(
					height_bytes  + i * map_width,
					height_bytes  + (i + 1 )* map_width,
					lineT[i]);
		std::copy(
					meta_bytes  + i * map_width,
					meta_bytes  + (i + 1 )* map_width,
					lineT[i] + map_width);
	}

	int32_t material_count = table_surface->w;
	uint8_t* material_begin_offsets = new uint8_t[material_count];
	uint8_t* material_end_offsets = new uint8_t[material_count];

	uint8_t* material_bytes = (uint8_t*)table_surface->pixels;
	for(int i = 0; i < material_count; i++){
		material_begin_offsets[i] = material_bytes[i * sizeof (uint32_t) + 0];
		material_end_offsets[i] = material_bytes[i * sizeof (uint32_t) + 1];
	}

	rv_gl_functor gl_functor = (rv_gl_functor)SDL_GL_GetProcAddress;


	rv_init_descriptor init_desc = {
		.width = window_width,
		.height = window_height,
		.gl_functor = gl_functor,
	};

	std::cout << "rv_init" << std::endl;
	rv_context context = rv_init(init_desc);

	rv_map_description map_desc {
		.width = map_width,
		.height = map_height,
		.lineT = lineT,
		.material_begin_offsets = material_begin_offsets,
		.material_end_offsets = material_end_offsets,
		.material_count = material_count,
	};

	std::cout << "rv_map_init" << std::endl;
	rv_map_init(context, map_desc);

	rv_rect update_region {
		.x = 0,
		.y = 0,
		.width = map_width,
		.height = map_height,
	};

	std::cout << "rv_map_exit" << std::endl;
	rv_map_exit(context);

	std::cout << "rv_map_init" << std::endl;
	rv_map_init(context, map_desc);

	std::cout << "rv_map_request_update" << std::endl;
	rv_map_request_update(context, update_region);

	rv_camera_description camera_desc {
		.fov = 43.0f,
		.aspect = (float)window_width / (float)window_height,
		.near = 0.1f,
		.far = 10000.0f,
	};

	std::cout << "rv_map_init" << std::endl;
	rv_camera_init(context, camera_desc);
	rv_rect viewport {
		.x = 0,
		.y = 0,
		.width = window_width,
		.height = window_height,
	};

	hmm_quaternion q = HMM_QuaternionFromAxisAngle(HMM_Vec3(1.0f, 0, 0), 90);

	rv_transform transform {
		.position = {
			.x = 1024,
			.y = 1024,
			.z = 512,
		},
		.rotation = {
			.x = q.X,
			.y = q.Y,
			.z = q.Z,
			.w = q.W,
		},

	};
	rv_camera_set_transform(context, transform);
	rv_render(context, viewport);
	rv_render(context, viewport);

	bool close = false;
	while (!close) {
		glClear( GL_COLOR_BUFFER_BIT );
		SDL_Event event;

		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {

			case SDL_QUIT:
				// handling of close button
				close = true;
				break;
			}
		}

		rv_render(context, viewport);

		SDL_GL_SwapWindow(window);

		SDL_Delay(1000 / 60);
	}

	rv_map_exit(context);
	rv_exit(context);
	SDL_FreeSurface(height_surface);
	SDL_FreeSurface(meta_surface);
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
