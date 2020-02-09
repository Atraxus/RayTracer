#pragma once

#include <glm/vec4.hpp>

class ShaderStorageBuffer
{
public:
	ShaderStorageBuffer(unsigned int const raycount, glm::vec4 data[]);
	~ShaderStorageBuffer();

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererID;
	

};

