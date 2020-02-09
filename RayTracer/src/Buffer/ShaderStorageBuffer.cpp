#include "ShaderStorageBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

ShaderStorageBuffer::ShaderStorageBuffer(unsigned int const raycount, glm::vec4 data[])
{
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing
	
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, raycount * sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
	glm::vec4* points = (glm::vec4*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, raycount * sizeof(glm::vec4), bufMask);
	points = data;
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
}

void ShaderStorageBuffer::Bind() const
{
}

void ShaderStorageBuffer::Unbind() const
{
}
