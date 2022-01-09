#ifndef SHADER_H
#define SHADER_H

#include <cstdint>

class Shader
{
public:
	Shader(const char* vs_shader, const char* fs_shader);
	void use();
	static void unuse();
	void set_uniform(const char* name, int value);
private:
	uint32_t _program;
};

#endif // SHADER_H
