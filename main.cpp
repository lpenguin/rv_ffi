#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <cstdint>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <fstream>
#include "glad/glad.h"
#include "m3d/Model.h"
#include <vector>

#include "vange_rs.h"

using namespace std;

float dot_vec3(rv_vector3 v1, rv_vector3 v2){
	return v1.x * v2.x +
			v1.y * v2.y +
			v1.z * v2.z;
}

rv_vector3 mul_vec3(rv_vector3 vector, float value){
	return {vector.x * value, vector.y * value, vector.z * value};
}

rv_vector3 div_vec3(rv_vector3 vector, float value){
	return {vector.x / value, vector.y / value, vector.z / value};
}

rv_quaternion rotation_quaternion(rv_vector3 axis, float angle_radians){
	rv_quaternion result;

	float axis_norm = sqrtf(dot_vec3(axis, axis));
	float sine_of_rotation = sinf(angle_radians / 2.0f);
	rv_vector3 rotated_vector = mul_vec3(axis, sine_of_rotation);

	rv_vector3 xyz = div_vec3(rotated_vector, axis_norm);
	result.x = xyz.x;
	result.y = xyz.y;
	result.z = xyz.z;
	result.w = cosf(angle_radians / 2.0f);

	return result;
}

const char * get_debug_type_str(GLenum type){
	switch(type){
	case GL_DEBUG_TYPE_ERROR:
		return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "PERFORMANCE";
	case GL_DEBUG_TYPE_OTHER:
		return "OTHER";
	case GL_DEBUG_TYPE_MARKER:
		return "MARKER";
	case GL_DEBUG_TYPE_POP_GROUP:
		return "POP_GROUP";
	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "PUSH_GROUP";
	default:
		return "Unknown";
	}
}

const char* get_debug_severity_str(GLenum severity){
	switch(severity){
	case GL_DEBUG_SEVERITY_HIGH:
		return "HIGH";
	case GL_DEBUG_SEVERITY_LOW:
		return "LOW";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "MEDIUM";
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "NOTIFICATION";
	default:
		return "Unknown";
	}
}

const char* get_debug_source_str(GLenum source){
	switch(source){
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:
		return "OTHER";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "THIRD_PARTY";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "WINDOW_SYSTEM";
	default:
		return "Unknown";
	}
}

void DebugCallbackARB(GLenum source,
					  GLenum type,
					  GLuint id,
					  GLenum severity,
					  GLsizei length,
					  const GLchar* message,
					  const GLvoid* userParam) {
	if(userParam != nullptr ||
	   type == GL_DEBUG_TYPE_ERROR)
	{
		std::cout << "GL Debug Message:"
				  << std::endl
				  << "\tseverity=" << get_debug_severity_str(severity)
				  << ", type=" << get_debug_type_str(type)
				  << ", source=" << get_debug_source_str(source)
				  << std::endl;

		std::cout << "\tmessage=" << std::string(message).substr(0, length) << std::endl;
	}
}

int main(int argc, char *argv[])
{
#ifdef WIN32
    // __chkStk check
    const int sz = 1024;
    char a[sz];
    char b[sz];
    char c[sz];
    char d[sz];
//    char e[sz];
#endif //  WIN32

	// make API version used
	if (rv_api_2 != 1) {
		return 1;
	}

	const int window_width = 1280;
	const int window_height = 720;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}
#ifdef USE_ANGLE
	SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
//	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);


	int img_init_flags = IMG_INIT_PNG;
	if((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags){
		printf("error initializing SDL2_image: %s\n", IMG_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("rv_ffi",
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   window_width,
									   window_height,
									   SDL_WINDOW_OPENGL);

    if(window == nullptr){
        std::cout << "Cannot create window: " << SDL_GetError() << std::endl;
        return 1;
    }

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	gladLoadGLES2Loader(SDL_GL_GetProcAddress);

    const uint8_t* gl_version = glGetString(GL_VERSION);
    std::cout << "GL version: " << gl_version << std::endl;

	if(glDebugMessageCallback != nullptr){
		glEnable(GL_DEBUG_OUTPUT);
		// pass here any value to catch only every debug message
		glDebugMessageCallback(&DebugCallbackARB, nullptr);
	}

	Object object;
	const char* model_path = "res/data/m1.m3d";
	std::FILE* f = std::fopen(model_path, "rb");
	if(f == nullptr){
		printf("error loading model \"%s\"\n", model_path);
		return 1;
	}
	read(f, object);

    const char* image_surface_path = "res/data/screen.png";
    SDL_Surface* image_surface = IMG_Load(image_surface_path);
    if(image_surface == nullptr){
        printf("error loading height image \"%s\": %s\n", image_surface_path, IMG_GetError());
        return 1;
    }

	GLenum gl_error;
	GLuint image_texture;
	glGenTextures(1, &image_texture);

	glBindTexture(GL_TEXTURE_2D, image_texture);
	gl_error = glGetError();
	if(gl_error != 0){
		std::cout << "glBindTexture error: " << gl_error << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_surface->w, image_surface->h, 0,
				 GL_RGBA, GL_UNSIGNED_BYTE, image_surface->pixels);
	gl_error = glGetError();
	if(gl_error != 0){
		std::cout << "glTexImage2D error: " << gl_error << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	const char* height_path = "res/data/height.png";
	SDL_Surface* height_surface = IMG_Load(height_path);
	if(height_surface == nullptr){
		printf("error loading height image \"%s\": %s\n", height_path, IMG_GetError());
		return 1;
	}

	const char* meta_path = "res/data/meta.png";
	SDL_Surface* meta_surface = IMG_Load(meta_path);
	if(meta_surface == nullptr){
		printf("error loading meta image \"%s\": %s\n", meta_path, IMG_GetError());
		return 1;
	}

	const char* table_path = "res/data/table.png";
	SDL_Surface* table_surface = IMG_Load(table_path);
	if(table_surface == nullptr){
		printf("error loading table image \"%s\": %s\n", table_path, IMG_GetError());
		return 1;
	}

	const char* palette_path = "res/data/palette.png";
	SDL_Surface* palette_surface = IMG_Load(palette_path);
	if(palette_surface == nullptr){
		printf("error loading palette image \"%s\": %s\n", palette_path, IMG_GetError());
		return 1;
	}

	uint8_t* height_bytes = (uint8_t*)height_surface->pixels;
	uint8_t* meta_bytes = (uint8_t*)meta_surface->pixels;

	int32_t map_width = height_surface->w;
	int32_t map_height = height_surface->h;

	const int32_t line_bottom = 0;
	const int32_t line_top = map_height;

	uint8_t** lineT = new uint8_t*[map_height];
	std::fill(lineT, lineT + map_height, (uint8_t*)0);

	for(int i = line_bottom; i < line_top; i++){
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

	    uint8_t* palette_data = (uint8_t*)palette_surface->pixels;
		uint8_t palette[0x300] = {};
		for(int i = 0; i < 0x300; i++){
			palette[i] = palette_data[i];
		}

	rv_map_description map_desc {
		.width = map_width,
		.height = map_height,
		.lineT = lineT,
		.material_begin_offsets = material_begin_offsets,
		.material_end_offsets = material_end_offsets,
		.material_count = material_count,
		.palette = palette,
	};

	std::cout << "rv_map_init" << std::endl;
	rv_map_init(context, map_desc);

	std::cout << "rv_map_update_palette" << std::endl;
	rv_map_update_palette(context, 0, 0x100, palette);

	rv_camera_description camera_desc {
		.fov = 43.0f,
		.aspect = (float)window_width / (float)window_height,
		.near = 10.0f,
		.far = 5000.0f,
	};

	std::cout << "rv_camera_init" << std::endl;
	rv_camera_init(context, camera_desc);
	rv_rect viewport {
		.x = 0,
		.y = 0,
		.width = window_width,
		.height = window_height,
	};


//	std::cout << "rv_map_exit" << std::endl;
//	rv_map_exit(context);

//	std::cout << "rv_map_init" << std::endl;
//	rv_map_init(context, map_desc);

	rv_rect update_region {
		.x = 0,
		.y = line_bottom,
		.width = map_width,
		.height = line_top - line_bottom,
	};
	std::cout << "rv_map_update_data" << std::endl;
	rv_map_update_data(context, update_region);


	float camera_angle = -M_PI * 0.1;
	float angle_step = M_PI/30.0f;
	float move_step = 10;

	rv_vector3 camera_position {
		.x = 1520,
		.y = 16090,
		.z = 712,
	};

	rv_quaternion camera_rotation = rotation_quaternion(rv_vector3{1.0f, .0f, .0f}, camera_angle);

	std::vector<uint64_t> model_handles;
	std::vector<uint64_t> instance_handles;

	const uint8_t color_id = 21;
	uint64_t body_handle = rv_model_create(context, "mechos1", &object.body);
	model_handles.push_back(body_handle);

	uint64_t body_instance_handle = rv_model_instance_create(context, body_handle, color_id);
	instance_handles.push_back(body_instance_handle);

	std::vector<rv_vector3> positions = {
		{1520, 15667, 30},
		{1378, 15383, 163},
		{1743, 15270, 110},
	};
	int32_t pos_index = 0;

//	char name[1024];
//	for(int i = 0; i < object.n_wheels; i++){
//		if(!object.wheels[i].steer) {
//			continue;
//		}
//		sprintf(name, "wheel %d", i);
//		uint64_t wheel_h = rv_model_create(context, name, &object.wheels[i].model);
//		model_handles.push_back(wheel_h);
//		rv_transform wheel_transform = rv_transform {
//			.position = rv_vector3 {
//				.x = model_transform.position.x + object.wheels[i].model.x_off,
//				.y = model_transform.position.y + object.wheels[i].model.y_off,
//				.z = model_transform.position.z + object.wheels[i].model.z_off,
//			},
//			.scale = model_transform.scale,
//			.rotation = model_transform.rotation,
//		};
//		uint64_t wheel_ih = rv_model_instance_create(context, wheel_h, color_id);
//		instance_handles.push_back(wheel_ih);
//		rv_model_instance_set_transform(context, wheel_ih, wheel_transform);
//	}

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

			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
					camera_position.y -= move_step;
					break;
				case SDL_SCANCODE_S:
					camera_position.y += move_step;
					break;
				case SDL_SCANCODE_A:
					camera_position.x -= move_step;
					break;
				case SDL_SCANCODE_D:
					camera_position.x += move_step;
					break;
				case SDL_SCANCODE_Q:
					camera_position.z -= move_step;
					break;
				case SDL_SCANCODE_E:
					camera_position.z += move_step;
					break;
				case SDL_SCANCODE_Z:
					camera_angle -= angle_step;
					camera_rotation = rotation_quaternion(rv_vector3{1.0f, .0f, .0f}, camera_angle);
					break;
				case SDL_SCANCODE_X:
					camera_angle += angle_step;
					camera_rotation = rotation_quaternion(rv_vector3{1.0f, .0f, .0f}, camera_angle);
					break;
				case SDL_SCANCODE_SPACE:
					pos_index = (pos_index + 1) % positions.size();
					break;
				default:
					break;
				}
				break;
			}


		}

		// Cycling the level
		if(camera_position.x < 0){
			camera_position.x = map_width + camera_position.x;
		} else if(camera_position.x > map_width ){
			camera_position.x = camera_position.x - map_width;
		}

		if(camera_position.y < 0){
			camera_position.y = map_height + camera_position.y;
		} else if(camera_position.y > map_height ){
			camera_position.y = camera_position.y - map_height;
		}

		rv_transform transform {
			.position = camera_position,
			.scale = 1,
			.rotation = camera_rotation,
		};

//		std::cout << "rv_camera_set_transform"
//			<< " position="
//			<< camera_position.x << " " << camera_position.y << " " << camera_position.z
//			<< ", rotation="
//			<< camera_rotation.x << " " << camera_rotation.y << " " << camera_rotation.z << " " << camera_rotation.w
//			<< std::endl;

		rv_camera_set_transform(context, transform);

//		float scale = 1;
		float scale = 0.24f;
//		float z_offset_of_mass_center = -60.260000f;
		float model_x = positions[pos_index].x;
		float model_y = positions[pos_index].y;
		float model_z = lineT[(int)model_y][(int)model_x];
		rv_transform model_transform = rv_transform {
			.position = rv_vector3 {
				.x = model_x,
				.y = model_y,
				.z = model_z
			},
			.scale = scale,
			.rotation = rotation_quaternion(rv_vector3{0.0f, 0.0f, 1.0f}, M_PI/2),
		};

		rv_model_instance_set_transform(context, body_instance_handle, model_transform);


//		std::cout << "rv_render" << std::endl;
		rv_render(context, viewport);

		gl_error = glGetError();
		if(gl_error != 0){
			std::cout << "rv_render error: " << gl_error << std::endl;
		}

		// reset unpack options
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);

		glBindTexture(GL_TEXTURE_2D, image_texture);
		gl_error = glGetError();
		if(gl_error != 0){
			std::cout << "glBindTexture error: " << gl_error << std::endl;
		}

		// Test Segfault
		glTexSubImage2D(GL_TEXTURE_2D,
						0,
						0, 0, image_surface->w, image_surface->h,
						 GL_RGBA, GL_UNSIGNED_BYTE, image_surface->pixels);

		gl_error = glGetError();
		if(gl_error != 0){
			std::cout << "glTexImage2D error: " << gl_error << std::endl;
		}

		SDL_GL_SwapWindow(window);

		SDL_Delay(1000 / 60);
	}

	for(uint64_t h: instance_handles){
		rv_model_instance_destroy(context, h);
	}

	for(uint64_t h: model_handles){
		rv_model_destroy(context, h);
	}

	rv_map_exit(context);
	rv_exit(context);

	std::cout << "rv done" << std::endl;

	SDL_FreeSurface(height_surface);
	SDL_FreeSurface(meta_surface);
	SDL_FreeSurface(table_surface);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(palette_surface);

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
