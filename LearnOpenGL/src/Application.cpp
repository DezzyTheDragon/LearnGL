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
		float positions[] =
		{
			100.0f, 100.0f, 0.0f, 0.0f,
			200.0f, 100.0f, 1.0f, 0.0f,
			200.0f, 200.0f, 1.0f, 1.0f,
			100.0f, 200.0f, 0.0f, 1.0f
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
		//Takes (left, right, top, bottom, near, far) and the numbers used are to create a projection that matches the window resolution
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));
		
		//Because we cant actually move the "camera" we have to move everything else
		//Model View Matrix is how he controll the placement and we multiply these three matracies
		//The order we multiply is Matrix, View, Model
		glm::mat4 mvp = proj * view * model;

		//Create shader and bind
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();

		//send information to the shader via uniforms
		//send the data
		shader.SetUniform4f("u_Color", 0.8f, 0.1f, 0.8f, 1.0f);

		shader.SetUniformMat4f("u_MVP", mvp);

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