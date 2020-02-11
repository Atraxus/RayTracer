#include "ShaderStorageBuffer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

ShaderStorageBuffer::ShaderStorageBuffer(const unsigned int size, void* data)
{
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing
	
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW); 
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

ShaderStorageBuffer::~ShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void ShaderStorageBuffer::Bind(unsigned int val) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, val, m_RendererID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
}

void ShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
