#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader/Shader.h"

class RayTracer {
public:
	RayTracer();
	~RayTracer();

	void run();

private: 
	Shader m_ComputeShader;


	void createComputeShader();
	void raytraceScene();
};