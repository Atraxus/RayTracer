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

//void drawScreenquad(Texture& textureToRender, const unsigned int sqID)
//{
//    glUseProgram(sqID);
//    glActiveTexture(GL_TEXTURE0 + textureToRender.GetBindPoint());
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_vertexUVBufferID);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(glm::vec4));
//
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
//
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//    glDisableVertexAttribArray(1);
//    glDisableVertexAttribArray(0);
//
//
//
//
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VerticesBuffer);
//
//    glBindImageTexture(0, HeightMap, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
//    glUseProgram(ComputeProgram);
//
//    glUniform1i(glGetUniformLocation(ComputeProgram, "HeightMap"), 0);
//
//
//    glDispatchCompute(1, 1, 1);
//    glMemoryBarrier(GL_ALL_BARRIER_BITS);
//}

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

		light light(glm::vec3(-2.5f, -2.5f, 2.5f), 1000.0f);
		for (int i = 0; i < 24; i++) {
			for (int j = 5; j < 8; j++) {
				positions[i * 9 + j] = light.getBrightness(glm::vec3(positions[i * 9], positions[i * 9 + 1], positions[i * 9 + 2]), positions[i * 9 + j]);
			}
		}

        VertexArray va;
        VertexBuffer vb(positions, 28 * 9 * sizeof(float)); // create vertex buffer with given vertices (positions) and the size of the given data

        VertexBufferLayout layout;
        layout.Push<float>(3); // Layout has 3 floats (in this case the x,y,z coordinates); Call Push again to tell layout that there are more information per vertex
        layout.Push<float>(2);
        layout.Push<float>(4);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 42); // create an index buffer with given indices and the number of indices

		Camera camera((glm::vec3)(0.0f, 0.0f, 0.0f), (glm::vec3)(0.0f, 0.0f, 0.0f));
        glm::mat4 Model = glm::mat4(1.0f);


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        //// stuff for testing texture
        //Texture texture("res/textures/feelsgoodman.jpg");
        //texture.Bind();
        //shader.SetUniform1i("u_Texture", 0);


        // unbind all buffers and shaders and vertex array
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        // create renderer
        Renderer renderer;
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if its closer to the camera than the former one
        glDepthFunc(GL_LESS);

    // --- ImGui stuff
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
    // ---




        
        
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

        Triangle* trisPtr = &triangles[0]; // vectors store their elements contiguously


        unsigned int numTriangles = triangles.size();
        ShaderStorageBuffer triangleSSBO(numTriangles * sizeof(Triangle), trisPtr);
        triangleSSBO.Bind(1);

        ComputeShader cs("res/shaders/Compute.shader");
        cs.Bind();

		//uniforms
		cs.SetUniform3f("camera.position", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		cs.SetUniform3f("camera.direction", camera.getViewDirection().x, camera.getViewDirection().y, camera.getViewDirection().z);

		//needs to be changed according to view direction
		cs.SetUniform3f("camera.xAxis", 1.0f, 0.0f, 0.0f);
		cs.SetUniform3f("camera.yAxis", 0.0f, 1.0f, 0.0f);

		cs.SetUniform1f("camera.tanFovY", (float)tan(glm::radians(45.0f)));
		cs.SetUniform1f("camera.tanFovX", (float)tan(glm::radians(45.0f)));
		//TODO: dynamic triangle count
		cs.SetUniform1i("triangleCount", 14);

		cs.SetUniform3f("light.position", light.getX(), light.getY(), light.getZ());
		cs.SetUniform1f("light.intensity", light.getIntensity());



        Texture textureToRender(1920, 1080);
        //Texture textureToRender("res/textures/feelsgoodman.jpg");
        textureToRender.Bind();

        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glDispatchCompute(1920 / 8, 1080 / 8, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        ScreenQuad screenQuad(shader);
        screenQuad.draw(textureToRender);
    // ---

        glm::vec3 translationA(0, 0, 0);
        float rotationXA = 0.0f;
        float rotationYA = 0.0f;
        float rotationZA = 0.0f;
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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            //// --- ImGui stuff
            //ImGui_ImplOpenGL3_NewFrame();
            //ImGui_ImplGlfw_NewFrame();
            //ImGui::NewFrame();


            screenQuad.draw(textureToRender);
            // ---

   //         //glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
   //         // View
			//camera.setPosition(glm::vec3(cameraX, cameraY, cameraZ));
			//camera.setFov(fov);
			//glm::mat4 View = camera.getView();
			//glm::mat4 Proj = camera.getProj();

   //         // Cube A
   //         {
   //             // Model
   //             glm::mat4 modelScaleMat = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
   //             glm::mat4 modelRotateMatX = glm::rotate(glm::mat4(1.0f), glm::radians(rotationXA), glm::vec3(1, 0, 0));
   //             glm::mat4 modelRotateMatY = glm::rotate(modelRotateMatX, glm::radians(rotationYA), glm::vec3(0, 1, 0));
   //             glm::mat4 modelRotateMat = glm::rotate(modelRotateMatY, glm::radians(rotationZA), glm::vec3(0, 0, 1));
   //             glm::mat4 modelTranslateMat = glm::translate(glm::mat4(1.0f), translationA);
   //             Model = modelTranslateMat * modelRotateMat * modelScaleMat;

   //             glm::mat4 mvp = Proj * View * Model; // inverted 

   //             shader.Bind();
   //             shader.SetUniform4Matf("u_MVP", mvp);
   //             renderer.Draw(va, ib, shader);
   //         }


   //         // --- ImGui stuff
   //         ImGui::SliderFloat("cameraX", &cameraX, -50.0f, 50.0f);
   //         if (ImGui::Button("cameraX = 0")) cameraX = 0.0f;
   //         ImGui::SliderFloat("cameraY", &cameraY, -50.0f, 50.0f);
   //         if (ImGui::Button("cameraY = 0")) cameraY = 0.0f;
   //         ImGui::SliderFloat("cameraZ", &cameraZ, -50.0f, 50.0f);
   //         if (ImGui::Button("cameraZ = 0")) cameraZ = 0.0f;
   //         // Cube A
   //         ImGui::SliderFloat("rotationXA", &rotationXA, -100.0f, 100.0f);
   //         if (ImGui::Button("rotationXA = 0")) rotationXA = 0.0f;

   //         ImGui::SliderFloat("RotationYA", &rotationYA, -100.0f, 100.0f);
   //         if (ImGui::Button("RotationYA = 0")) rotationYA = 0.0f;

   //         ImGui::SliderFloat("RotationZA", &rotationZA, -100.0f, 100.0f);
   //         if (ImGui::Button("RotationZA = 0")) rotationZA = 0.0f;

   //         ImGui::SliderFloat("AX", &translationA.x, -50.0f, 50.0f);
   //         if (ImGui::Button("AX = 0")) translationA.x = 0.0f;

   //         ImGui::SliderFloat("AY", &translationA.y, -50.0f, 50.0f);
   //         if (ImGui::Button("AY = 0")) translationA.y = 0.0f;

   //         ImGui::SliderFloat("AZ", &translationA.z, -50.0f, 50.0f);
   //         if (ImGui::Button("AZ = 0")) translationA.z = 0.0f;

			//
   //         ImGui::SliderFloat("FOV", &fov, 0.0f, 180.0f);
			//if (ImGui::Button("FOV = 45")) fov = 45.0f;
   //         ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   //         ImGui::Render();
   //         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   //         // ---

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    /*ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();*/
    glfwTerminate();
    return 0;
}