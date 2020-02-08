#pragma once

class ColorBuffer
{
public:
	ColorBuffer(const void* data, unsigned int count);
	~ColorBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Size; };

private:
	unsigned int m_RendererID;
	unsigned int m_Size;
};