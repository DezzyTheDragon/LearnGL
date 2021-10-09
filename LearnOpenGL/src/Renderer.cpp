#include "Renderer.h"
#include <iostream>

//clear the error flags in openGL
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

//prints the errors that occor
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL_Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}


void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);

}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	ib.Bind();
	va.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));	//for using with a vertex buffer (GLCall is a macro I wrote, glDrawElements is the function)

}