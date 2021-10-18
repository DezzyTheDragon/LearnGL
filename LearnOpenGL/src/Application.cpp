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

	//create a scope so that when the window terminates the program does not get stuck in an infanit loop when
	//deconstructing opengl objects that no longer have a valid opengl context
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

		//Create shader and bind
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		//send information to the shader via uniforms
		//send the data
		//shader.SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);

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
		//float r = 0.0f;
		//float incriment = 0.05f;

		Renderer renderer;

		//loop
		while (!glfwWindowShouldClose(window))
		{
			//Clear
			renderer.Clear();

			//Draw

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.1f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);
			

			//animate color
			/*
			if (r > 1.0f)
			{
				incriment = -0.05f;
			}
			else if (r < 0.0f)
			{
				incriment = 0.05f;
			}

			r += incriment;
			*/

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

	}

	glfwTerminate();

	return 0;
}