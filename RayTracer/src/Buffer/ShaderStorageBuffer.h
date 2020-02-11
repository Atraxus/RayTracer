#pragma once

#include <glm/vec4.hpp>

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(const unsigned int size, void* data);
	~ShaderStorageBuffer();

	void Bind(unsigned int val) const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
	

};

