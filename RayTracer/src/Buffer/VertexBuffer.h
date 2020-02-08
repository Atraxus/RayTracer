#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int sizte);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
};