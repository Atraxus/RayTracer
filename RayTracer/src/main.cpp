// opengl and glfw3 include
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// math library include
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// includes for imgui (not relevant for project at the end)
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// c++ std includes
#include <iostream>

// project file includes
#include "Renderer/Renderer.h"
#include "Buffer/VertexBuffer.h"
#include "Buffer/ColorBuffer.h"
#include "Buffer/VertexBufferLayout.h"
#include "Buffer/IndexBuffer.h"
#include "VertexArray.h"
#include "Shader/Shader.h"
#include "Shader/ComputeShader.h"
#include "Buffer/ShaderStorageBuffer.h"
#include "Texture/Texture.h"
#include "ScreenQuad.h"
#include "Camera/Camera.h"
#include "Light/light.h"

// Structs
struct Triangle {
    glm::vec3 pointA, pointB, pointC;
    glm::vec4 color;
};

// ---------------- MAIN ----------------
int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1080, "RayTracer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // initialize OpenGL
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: GLEW init failed!" << std::endl;
        return -1;
    }

    // print version to console
    std::cout << glGetString(GL_VERSION) << std::endl;
    { // scope because of stack allocated vb and ib lead to infinite loop with glGetError

        float positions[] =
        {
            // Cube A
                //front
                -2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                 2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                -2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                 2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                //bottom
                -2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                 2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                -2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                 2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                //left
                -2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                //right
                 2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                //top
                -2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                -2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                 2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                 2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                //back
                -2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                 2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                -2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                 2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,

                 //bottom
                -100.0f, -3.0f,  100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                 100.0f, -3.0f,  100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                -100.0f, -3.0f, -100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                 100.0f, -3.0f, -100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f
        };

        unsigned int indices[] = // has to be unsigned
        {
			//front
            0, 1, 2,
            1, 3, 2,
			//bottom
			4, 5, 6,
			5, 6, 7,
			//left
			8, 9, 10,
			9, 10, 11,
			//right
			12, 13, 14,
			13, 14, 15,
			//top
			16, 17, 18,
			17, 18, 19,
			//back
			20, 21, 22,
			21, 22, 23,

            //back
            24, 25, 26,
            25, 26, 27
        };

		light light(glm::vec3(0.0f, 5.0f, 10.0f), 1000.0f);
		for (int i = 0; i < 24; i++) {
			for (int j = 5; j < 8; j++) {
				positions[i * 9 + j] = light.getBrightness(glm::vec3(positions[i * 9], positions[i * 9 + 1], positions[i * 9 + 2]), positions[i * 9 + j]);
			}
		}

        Camera camera(1920, 1080, 70.0f, glm::vec3(0.0f,30.0f,0.0f), 
            glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 Model = glm::mat4(1.0f);
        
    // --- Compute shader stuff
        std::vector<Triangle> triangles;
        Triangle triangle;
        for (int i = 0; i < 42; i+=3) {
            int ia = indices[i];
            int ib = indices[i + 1];
            int ic = indices[i + 2];

            float x = positions[ia * 9];
            float y = positions[ia * 9 + 1];
            float z = positions[ia * 9 + 2];
            triangle.pointA = glm::vec3(x, y, z);

            x = positions[ib * 9];
            y = positions[ib * 9 + 1];
            z = positions[ib * 9 + 2];
            triangle.pointB = glm::vec3(x, y, z);

            x = positions[ic * 9];
            y = positions[ic * 9 + 1];
            z = positions[ic * 9 + 2];
            triangle.pointC = glm::vec3(x, y, z);

            float r = positions[ia * 9 + 5];
            float g = positions[ia * 9 + 6];
            float b = positions[ia * 9 + 7];
            float a = positions[ia * 9 + 8];
            triangle.color = glm::vec4(r, g, b, a);


            triangles.push_back(triangle);
        }

        unsigned int numTriangles = triangles.size();
        Triangle* trisPtr = &triangles[0]; // vectors store their elements contiguously
        ShaderStorageBuffer triangleSSBO(numTriangles * sizeof(Triangle), trisPtr);
        triangleSSBO.Bind(1);

        ComputeShader cs("res/shaders/Compute.shader");
        cs.Bind();

		//uniforms
		cs.SetUniform3f("camera.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		cs.SetUniform3f("camera.direction", camera.getViewDirection().x, camera.getViewDirection().y, camera.getViewDirection().z);

		//needs to be changed according to view direction
		cs.SetUniform3f("camera.xAxis", camera.getRight().x, camera.getRight().y, camera.getRight().z);
		cs.SetUniform3f("camera.yAxis", camera.getUp().x, camera.getUp().y, camera.getUp().z);

		cs.SetUniform1f("camera.tanFovY", camera.getFovY());
		cs.SetUniform1f("camera.tanFovX", camera.getFovY());
		//TODO: dynamic triangle count
		cs.SetUniform1i("triangleCount", numTriangles);

		cs.SetUniform3f("light.position", light.getX(), light.getY(), light.getZ());
		cs.SetUniform1f("light.intensity", light.getIntensity());

        Texture textureToRender(1920, 1080);
        //Texture textureToRender("res/textures/feelsgoodman.jpg");
        textureToRender.Bind();

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glDispatchCompute(1920 / 8, 1080 / 8, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        ScreenQuad screenQuad(shader);
    // --- Compute shader stuff

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			screenQuad.draw(textureToRender);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}