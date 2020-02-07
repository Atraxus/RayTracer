#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
public:
	Shader(const std::string& filepath);
	~Shader();


	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4Matf(const std::string& name, glm::mat4 proj);

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);

	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};