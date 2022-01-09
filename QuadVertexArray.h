#ifndef TEXTUREPIPELINE_H
#define TEXTUREPIPELINE_H
#include <cstdint>

class QuadVertexArray
{
public:
	QuadVertexArray();
	void bind();
	static void unbind();
private:
	uint32_t VAO;
};

#endif // TEXTUREPIPELINE_H
