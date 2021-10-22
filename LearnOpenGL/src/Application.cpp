#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//This is the main file for learning openGL project


int main()
{
	GLFWwindow* window;

	//init glfw
	if (!glfwInit())
	{
		return -1;
	}

	//set openGL profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//create the window
	window = glfwCreateWindow(960, 540, "Learn OpenGL", NULL, NULL);

	//verify window exists before trying to use it
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make window current context
	glfwMakeContextCurrent(window);

	//sets the swap to match the display refresh rate
	glfwSwapInterval(1);

	//Init glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	//These curly braces create a scope so that when the window terminates the program does not 
	//get stuck in an infanit loop when deconstructing opengl objects that no longer have a valid opengl context
	{
		//Allows blending of alpha pixles for textures
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));
		
		Renderer renderer;

		//NOTE: this needs to be in the same context, can not init outside of the current context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		const char* glsl_version = "#version 330 core";

		//true to auto install callback, if false I have to handle the callback
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestTexture>("Texture Test");
		
		
		//loop
		while (!glfwWindowShouldClose(window))
		{	
			//Clear
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			//Draw
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();	
			
			if (currentTest)
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			glfwPollEvents();

		}

	}

	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}