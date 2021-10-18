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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
	window = glfwCreateWindow(640, 480, "Learn OpenGL", NULL, NULL);

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
		float positions[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		//Allows blending of alpha pixles for textures
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_BLEND));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		//create a projection matrix so that the 3d world can accuratly be cast onto the 2d pland that is the screen
		//set to an orthographic view
		//Takes (left, right, top, bottom, near, far) and the numbers used are to create
		//a screen that has a 4:3 aspect ratio to match the resolution of the window (640, 480)
		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

		//Create shader and bind
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		//send information to the shader via uniforms
		//send the data
		shader.SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);

		shader.SetUniformMat4f("u_MVP", proj);

		//create a texture and bind
		Texture texture("res/textures/image.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		//unbind everything
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		//animate the color
		float r = 0.0f;
		float incriment = 0.05f;

		Renderer renderer;

		//loop
		while (!glfwWindowShouldClose(window))
		{
			//Clear
			renderer.Clear();

			//Draw

			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.1f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);
			

			//animate color
			
			if (r > 1.0f)
			{
				incriment = -0.05f;
			}
			else if (r < 0.0f)
			{
				incriment = 0.05f;
			}

			r += incriment;
			

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

	}

	glfwTerminate();

	return 0;
}