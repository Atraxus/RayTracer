// opengl and glfw3 include
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// math library include
#include "glm/gtc/matrix_transform.hpp"

// includes for imgui (not relevant for project at the end)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// c++ std includes
#include <iostream>

// project file includes
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

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
           -5.0f, -5.0f, -20.0f, 0.0f, 0.0f,
            5.0f, -5.0f, -20.0f, 0.0f, 0.0f,
		   -5.0f,  5.0f, -20.0f, 0.0f, 0.0f,
		    5.0f,  5.0f, -20.0f, 0.0f, 0.0f,
		   -5.0f, -5.0f, -30.0f, 0.0f, 0.0f,
		    5.0f, -5.0f, -30.0f, 0.0f, 0.0f,
		   -5.0f,  5.0f, -30.0f, 0.0f, 0.0f,
		    5.0f,  5.0f, -30.0f, 0.0f, 0.0f,
        };
        unsigned int indices[] = // has to be unsigned
        {
			//vorne
            0, 1, 2,
            1, 3, 2,
			//unten
			4, 1, 0,
			5, 4, 1,
			//links
			4, 0, 2,
			4, 2, 6,
			//rechts
			1, 5, 3,
			3, 5, 7,
			//oben
			2, 3, 6,
			3, 7, 6,
			//hinten
			6, 5, 4,
			7, 5, 6
        };

        VertexArray va;
        VertexBuffer vb(positions, 8 * 5 * sizeof(float)); // create vertex buffer with given vertices (positions) and the size of the given data

        VertexBufferLayout layout;
        layout.Push<float>(3); // Layout has 3 floats (in this case the x,y,z coordinates); Call Push again to tell layout that there are more information per vertex
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 36); // create an index buffer with given indices and the number of indices


		/*Camera camera;
		glm::mat4 proj = glm::perspective(glm::radians(45.0f),(1920.0f/1080.0f), 100.0f, 1000.0f);
		glm::mat4 oProj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -200.0f, 200.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));*/

        glm::mat4 Proj = glm::perspective(glm::radians(45.f), 1.33f, 0.1f, 100.f);
        glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), glm::vec3(1));
        glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, 0.0f, glm::vec3(1, 0, 0));
        glm::mat4 View = glm::rotate(ViewRotateX, 0.0f, glm::vec3(0, 1, 0));
        glm::mat4 Model = glm::mat4(1.0f);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
		//shader.SetUniform4Matf("u_MVP", mvp);
		//shader.SetUniform4Matf("u_View", view);

        // stuff for testing texture
        Texture texture("res/textures/feelsgoodman.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);


        // unbind all buffers and shaders and vertex array
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        // create renderer
        Renderer renderer;

        // --- ImGui stuff
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
        // ---


        glm::vec3 translation(0, 0, 0);
        float rotationX = 0.0f;
        float rotationY = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            // --- ImGui stuff
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // ---

            //glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

            ViewTranslate = glm::translate(glm::mat4(1.0f), translation);
            ViewRotateX = glm::rotate(ViewTranslate, rotationX, glm::vec3(1, 0, 0));
            View = glm::rotate(ViewRotateX, rotationY, glm::vec3(0, 1, 0));
            Model = glm::mat4(1.0f);
            glm::mat4 mvp = Proj * View * Model;

            shader.Bind();
            shader.SetUniform4Matf("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
            
            // --- ImGui stuff
            ImGui::SliderFloat("RotationX", &rotationX, -5.0f, 5.0f);
            if (ImGui::Button("RotationX = 0")) rotationX = 0.0f;

            ImGui::SliderFloat("RotationY", &rotationY, -5.0f, 5.0f);
            if (ImGui::Button("RotationY = 0")) rotationY = 0.0f;

            ImGui::SliderFloat("X", &translation.x, -50.0f, 50.0f);
            if (ImGui::Button("X = 0")) translation.x = 0.0f;

            ImGui::SliderFloat("Y", &translation.y, -50.0f, 50.0f);
            if (ImGui::Button("Y = 0")) translation.y = 0.0f;

            ImGui::SliderFloat("Z", &translation.z, -50.0f, 50.0f);
            if (ImGui::Button("Z = 0")) translation.z = 0.0f;

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            // ---

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