#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"


//Constructor for shader
//Takes: shader file path
Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	 
}

//Deleates the shader program
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

//Reads the file and compiles the shader
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	//create shader
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	//specify source code
	glShaderSource(id, 1, &src, nullptr);
	//compile the shader
	glCompileShader(id);

	//Error handeling
	//check the CompileShader for the status
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//allocate memory for error message
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return 0;
	}

	return id;
}

//enables the shader program
void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

//disables the shader program
void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

//Basic setter for uniform4f
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

//gets the given variable location in shader program
unsigned int Shader::GetUniformLocation(const std::string& name)
{
	//holds the location
	int location;
	//check if the location has already been cached
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		//if cached then set location to the cached value
		location = m_UniformLocationCache[name];
	}
	else
	{
		//if not cached then get the locaiton
		GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
		//check to make sure its there
		if (location == -1)
		{
			std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;
		}
		//add to the cache
		m_UniformLocationCache[name] = location;
		
	}
	//return the location
	return location;
}


//Instead of using two different files for the vertex shader and fragment shader
//we combine the two in a single shader for example Basic.shader
//this make writing the shaders easier. In addition to reading in the files
//we also split the files up because openGL still needs two files
ShaderProgramSource Shader::ParseShader(const std::string filepath)
{
	std::ifstream stream(filepath);

	//enum for what type of shader we have
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		//check if the line has the #shader keyword
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				//vertex file
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				//fragment file
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			//use the type as an index into the array
			ss[(int)type] << line << '\n';
		}
	}

	//return the source
	return { ss[0].str(), ss[1].str() };

}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//create program
	unsigned int program = glCreateProgram();
	//compile shader
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//attach shaders to the program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//link the program
	glLinkProgram(program);
	glValidateProgram(program);

	//We can now delete the shaders because they are linked to a program
	//Shaders are like the intermediates for the final code
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

