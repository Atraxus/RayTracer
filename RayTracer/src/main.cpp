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

#include "ComputeSimulator.hpp"

// Structs
struct Triangle {
    glm::vec3 pointA, pointB, pointC;
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
                -2.5f, -2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                 2.5f, -2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                -2.5f,  2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                 2.5f,  2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.1f, 0.1f, 1.0f,
                //bottom
                -2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                 2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                -2.5f, -2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                 2.5f, -2.5f,  -7.5f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 1.0f,
                //left
                -2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f, -2.5f, -7.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                -2.5f,  2.5f, -7.5f, 0.0f, 0.0f, 0.1f, 0.8f, 0.1f, 1.0f,
                //right
                 2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f, -2.5f, -7.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                 2.5f,  2.5f, -7.5f, 0.0f, 0.0f, 0.1f, 0.1f, 0.8f, 1.0f,
                //top
                -2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                 2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                -2.5f,  2.5f, -7.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                 2.5f,  2.5f, -7.5f, 0.0f, 0.0f, 0.6f, 0.6f, 0.1f, 1.0f,
                //back
                -2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                 2.5f, -2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                -2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,
                 2.5f,  2.5f, -12.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f,

                 //bottom
                -100.0f, -3.0f, -100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                 100.0f, -3.0f, -100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                -100.0f, -3.0f,  100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f,
                 100.0f, -3.0f,  100.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 1.0f
        };

        unsigned int indices[] = // has to be unsigned
        {
			//front
            0, 1, 2,
            1, 3, 2,
			//bottom
			4, 5, 6,
			5, 7, 6,
			//left
			8, 9, 10,
			9, 11, 10,
			//right
			12, 13, 14,
			13, 15, 14,
			//top
			16, 17, 18,
			17, 19, 18,
			//back
			20, 21, 22,
			21, 23, 22,

            //back
            24, 25, 26,
            25, 27, 26
        };

		light light(glm::vec3(0.0f, 20.0f, 0.0f), 100.0f);

        Camera camera(1920, 1080, 90.0f, glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 Model = glm::mat4(1.0f);
        
    // --- Compute shader stuff
        glm::vec3 triangleAs[14];
        glm::vec3 triangleBs[14];
        glm::vec3 triangleCs[14];
        glm::vec4 colors[14];
        for (int i = 0; i < 42; i+=3) {
            int ia = indices[i];
            int ib = indices[i + 1];
            int ic = indices[i + 2];

            float x = positions[ia * 9];
            float y = positions[ia * 9 + 1];
            float z = positions[ia * 9 + 2];
            triangleAs[i / 3] = glm::vec3(x, y, z);

            x = positions[ib * 9];
            y = positions[ib * 9 + 1];
            z = positions[ib * 9 + 2];
            triangleBs[i / 3] = glm::vec3(x, y, z);

            x = positions[ic * 9];
            y = positions[ic * 9 + 1];
            z = positions[ic * 9 + 2];
            triangleCs[i / 3] = glm::vec3(x, y, z);

            float r = positions[ia * 9 + 5];
            float g = positions[ia * 9 + 6];
            float b = positions[ia * 9 + 7];
            float a = positions[ia * 9 + 8];

            colors[i / 3] = glm::vec4(r, g, b, a);
        }

        ShaderStorageBuffer aPointSSBO(14 * sizeof(glm::vec3), triangleAs);
        aPointSSBO.Bind(1);
        ShaderStorageBuffer bPointSSBO(14 * sizeof(glm::vec3), triangleBs);
        bPointSSBO.Bind(2);
        ShaderStorageBuffer cPointSSBO(14 * sizeof(glm::vec3), triangleCs);
        cPointSSBO.Bind(3);
        ShaderStorageBuffer colorsSSBO(14 * sizeof(glm::vec4), colors);
        colorsSSBO.Bind(4);

        ComputeShader cs("res/shaders/Compute.shader");
        cs.Bind();

		//uniforms
		cs.SetUniform3f("camera.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		cs.SetUniform3f("camera.direction", camera.getViewDirection().x, camera.getViewDirection().y, camera.getViewDirection().z);

		//needs to be changed according to view direction
		cs.SetUniform3f("camera.xAxis", -(camera.getRight().x), camera.getRight().y, camera.getRight().z);
		cs.SetUniform3f("camera.yAxis", camera.getUp().x, -(camera.getUp().y), camera.getUp().z);

		cs.SetUniform1f("camera.tanFovY", camera.getFovY());
		cs.SetUniform1f("camera.tanFovX", camera.getFovX());
		//TODO: dynamic triangle count
		cs.SetUniform1i("triangleCount", 14);

		cs.SetUniform3f("light.position", light.getX(), light.getY(), light.getZ());
		cs.SetUniform1f("light.intensity", light.getIntensity());

		cs.SetUniform1i("width", 1920);
		cs.SetUniform1i("height", 1080);
		cs.SetUniform1ui("reflectionDepth", 1);

        Texture textureToRender(1920, 1080);
        //Texture textureToRender("res/textures/feelsgoodman.jpg");
        textureToRender.Bind();


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.Unbind();
        textureToRender.Unbind();
        cs.Unbind();
        aPointSSBO.Unbind();
        bPointSSBO.Unbind();
        cPointSSBO.Unbind();
        colorsSSBO.Unbind();




        // ------- TESTING -------

        /*ComputeSimulator compSim(trisPtr, tan(glm::radians(45.0f)), tan(glm::radians(45.0f)), (int)numTriangles, 1920, 1080);
        compSim.run();*/


        // -----------------------

        ScreenQuad screenQuad(shader);
    // --- Compute shader stuff

    // --- ImGui stuff
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
    // ---

        glm::vec3 newViewDir = camera.getViewDirection();
        glm::vec3 newPos(0.0f, 0.0f, -15.0f);
        
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            // --- ImGui stuff
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::SliderFloat("View direction (X)", &newViewDir.x, -1.0f, 1.0f);
            if (ImGui::Button("View direction (X) = 0")) newViewDir.x = 0.0f;

            ImGui::SliderFloat("View direction (y)", &newViewDir.y, -1.0f, 1.0f);
            if (ImGui::Button("View direction (y) = 0")) newViewDir.y = 0.0f;

            ImGui::SliderFloat("View direction (z)", &newViewDir.z, -1.0f, 1.0f);
            if (ImGui::Button("View direction (z) = 0")) newViewDir.z = 0.0f;

            ImGui::SliderFloat("New Pos (X)", &newPos.x, -50.0f, 50.0f);
            if (ImGui::Button("New Pos (X) = 0")) newPos.x = 0.0f;

            ImGui::SliderFloat("New Pos (y)", &newPos.y, -50.0f, 50.0f);
            if (ImGui::Button("New Pos (y) = 0")) newPos.y = 0.0f;

            ImGui::SliderFloat("New Pos (z)", &newPos.z, -50.0f, 50.0f);
            if (ImGui::Button("New Pos (z) = 0")) newPos.z = 0.0f;

            camera.setViewDirection(newViewDir);
            camera.setPosition(newPos);

            //std::cout << camera.getViewDirection().x << " " << camera.getViewDirection().y << " " << camera.getViewDirection().z << std::endl;


            
			cs.Bind();
			cs.SetUniform3f("camera.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
			cs.SetUniform3f("camera.direction", camera.getViewDirection().x, camera.getViewDirection().y, camera.getViewDirection().z);
			//needs to be changed according to view direction
			cs.SetUniform3f("camera.xAxis", (camera.getRight().x), camera.getRight().y, camera.getRight().z);
			cs.SetUniform3f("camera.yAxis", camera.getUp().x, (camera.getUp().y), camera.getUp().z);

            textureToRender.Bind();
            aPointSSBO.Bind(1);
            bPointSSBO.Bind(2);
            cPointSSBO.Bind(3);
            colorsSSBO.Bind(4);

            //glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glDispatchCompute(1920 / 16, 1080 / 16, 1);
            //glMemoryBarrier(GL_ALL_BARRIER_BITS);

            /*triangleSSBO.Bind(1);
            Triangle* returnTriangles = (Triangle*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);*/

            shader.Bind();
			screenQuad.draw(textureToRender);


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}