#pragma once

#include "../Renderer/Renderer.h"

class Texture
{
public:
	Texture(const std::string& path);
	Texture(const unsigned int& width, const unsigned int& height);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetRendererID() const { return m_RendererID; };
	inline int GetBindPoint() const { return m_Bindpoint; };

private:
	unsigned int m_RendererID;
	unsigned int m_Bindpoint;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

};