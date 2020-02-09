// opengl and glfw3 include
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// math library include
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>

// includes for imgui (not relevant for project at the end)
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include "Texture/Texture.h"
#include "Camera/Camera.h"

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
        /*float positions[] =
        {
           -2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.583f,  0.771f,  0.014f, 1.0f,
            2.5f, -2.5f,  2.5f, 0.0f, 0.0f, 0.327f,  0.483f,  0.844f, 1.0f,
           -2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.327f,  0.0f,    0.844f, 1.0f,
            2.5f,  2.5f,  2.5f, 0.0f, 0.0f, 0.822f,  0.569f,  0.201f, 1.0f,
           -2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.0f,    0.602f,  0.223f, 1.0f,
            2.5f, -2.5f, -2.5f, 0.0f, 0.0f, 0.310f,  0.747f,  0.0f,   1.0f,
           -2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.0,     0.616f,  0.489f, 1.0f,
            2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.559f,  0.436f,  0.730f, 1.0f
        };*/

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
                 2.5f,  2.5f, -2.5f, 0.0f, 0.0f, 0.1f, 0.6f, 0.6f, 1.0f
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
			21, 22, 23
        };

        VertexArray va;
        VertexBuffer vb(positions, 24 * 9 * sizeof(float)); // create vertex buffer with given vertices (positions) and the size of the given data

        VertexBufferLayout layout;
        layout.Push<float>(3); // Layout has 3 floats (in this case the x,y,z coordinates); Call Push again to tell layout that there are more information per vertex
        layout.Push<float>(2);
        layout.Push<float>(4);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 36); // create an index buffer with given indices and the number of indices

		Camera camera((glm::vec3)(0.0f, 0.0f, 0.0f), (glm::vec3)(0.0f, 0.0f, 0.0f));
		/*
		glm::mat4 proj = glm::perspective(glm::radians(45.0f),(1920.0f/1080.0f), 100.0f, 1000.0f);
		glm::mat4 oProj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -200.0f, 200.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));*/

        /*glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), glm::vec3(1));
        glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, 0.0f, glm::vec3(1, 0, 0));
        glm::mat4 View = glm::rotate(ViewRotateX, 0.0f, glm::vec3(0, 1, 0));*/
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
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // --- ImGui stuff
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
        // ---


        glm::vec3 translationA(0, 0, 0);
        /*glm::vec3 translationB(0, 0, 0);*/
        float rotationXA = 0.0f;
        float rotationYA = 0.0f;
        float rotationZA = 0.0f;
        //float rotationXB = 0.0f;
        //float rotationYB = 0.0f;
        //float rotationZB = 0.0f;
        float cameraX = 0.0f;
        float cameraY = 1.0f;
        float cameraZ = 10.0f;
		float fov = 45.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //renderer.Clear();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

            // --- ImGui stuff
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // ---

            //glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            // View
			camera.setPosition(glm::vec3(cameraX, cameraY, cameraZ));
			camera.setFov(fov);
			glm::mat4 View = camera.getView();
			glm::mat4 Proj = camera.getProj();

            // Cube A
            {
                // Model
                glm::mat4 modelScaleMat = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
                glm::mat4 modelRotateMatX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationXA), glm::vec3(1, 0, 0));
                glm::mat4 modelRotateMatY = glm::rotate(modelRotateMatX, glm::radians(rotationYA), glm::vec3(0, 1, 0));
                glm::mat4 modelRotateMat = glm::rotate(modelRotateMatY, glm::radians(rotationZA), glm::vec3(0, 0, 1));
                glm::mat4 modelTranslateMat = glm::translate(glm::mat4(1.0f), translationA);
                Model = modelTranslateMat * modelRotateMat * modelScaleMat;

                glm::mat4 mvp = Proj * View * Model; // inverted 

                shader.Bind();
                shader.SetUniform4Matf("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            //// Cube B
            //{
            //    // Model
            //    glm::mat4 modelScaleMat = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
            //    glm::mat4 modelRotateMatX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationXB), glm::vec3(1, 0, 0));
            //    glm::mat4 modelRotateMatY = glm::rotate(modelRotateMatX, glm::radians(rotationYB), glm::vec3(0, 1, 0));
            //    glm::mat4 modelRotateMat = glm::rotate(modelRotateMatY, glm::radians(rotationZB), glm::vec3(0, 0, 1));
            //    glm::mat4 modelTranslateMat = glm::translate(glm::mat4(1.0f), translationB);
            //    Model = modelTranslateMat * modelRotateMat * modelScaleMat;


            //    glm::mat4 mvp = Proj * View * Model; // inverted 

            //    shader.SetUniform4Matf("u_MVP", mvp);
            //    renderer.Draw(va, ib, shader);
            //}



            // --- ImGui stuff
            ImGui::SliderFloat("cameraX", &cameraX, -50.0f, 50.0f);
            if (ImGui::Button("cameraX = 0")) cameraX = 0.0f;
            ImGui::SliderFloat("cameraY", &cameraY, -50.0f, 50.0f);
            if (ImGui::Button("cameraY = 0")) cameraY = 0.0f;
            ImGui::SliderFloat("cameraZ", &cameraZ, -50.0f, 50.0f);
            if (ImGui::Button("cameraZ = 0")) cameraZ = 0.0f;
            // Cube A
            ImGui::SliderFloat("rotationXA", &rotationXA, -100.0f, 100.0f);
            if (ImGui::Button("rotationXA = 0")) rotationXA = 0.0f;

            ImGui::SliderFloat("RotationYA", &rotationYA, -100.0f, 100.0f);
            if (ImGui::Button("RotationYA = 0")) rotationYA = 0.0f;

            ImGui::SliderFloat("RotationZA", &rotationZA, -100.0f, 100.0f);
            if (ImGui::Button("RotationZA = 0")) rotationZA = 0.0f;

            ImGui::SliderFloat("AX", &translationA.x, -50.0f, 50.0f);
            if (ImGui::Button("AX = 0")) translationA.x = 0.0f;

            ImGui::SliderFloat("AY", &translationA.y, -50.0f, 50.0f);
            if (ImGui::Button("AY = 0")) translationA.y = 0.0f;

            ImGui::SliderFloat("AZ", &translationA.z, -50.0f, 50.0f);
            if (ImGui::Button("AZ = 0")) translationA.z = 0.0f;

            //// Cube B
            //ImGui::SliderFloat("rotationXB", &rotationXB, -50.0f, 50.0f);
            //if (ImGui::Button("rotationXB = 0")) rotationXB = 0.0f;

            //ImGui::SliderFloat("RotationYB", &rotationYB, -50.0f, 50.0f);
            //if (ImGui::Button("RotationYB = 0")) rotationYB = 0.0f;

            //ImGui::SliderFloat("RotationZB", &rotationZB, -50.0f, 50.0f);
            //if (ImGui::Button("RotationZB = 0")) rotationZB = 0.0f;

            //ImGui::SliderFloat("BX", &translationB.x, -50.0f, 50.0f);
            //if (ImGui::Button("BX = 0")) translationB.x = 0.0f;

            //ImGui::SliderFloat("BY", &translationB.y, -50.0f, 50.0f);
            //if (ImGui::Button("BY = 0")) translationB.y = 0.0f;

            //ImGui::SliderFloat("BZ", &translationB.z, -50.0f, 50.0f);
            //if (ImGui::Button("BZ = 0")) translationB.z = 0.0f;

			
            ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f);
			if (ImGui::Button("FOV = 45")) fov = 45.0f;
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