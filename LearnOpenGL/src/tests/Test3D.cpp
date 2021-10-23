#include "Test3D.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
	Test3D::Test3D() : m_proj(glm::perspective(45.0f, 960.0f/540.0f, 1.0f, 150.0f)),
						m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
						m_transform(glm::vec3(0.0f, 0.0f, -100.0f)),
						m_rotation(glm::vec3(0.0f, 0.0f, 0.0f))
	{
		//NOTE: remember this array is the object itself. DO NOT TRANSFORM 
		float positions[] = {
			-25.0f, -25.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 25.0f, -25.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			 25.0f,  25.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			-25.0f,  25.0f, 0.0f, 0.0f, 0.0f, -1.0f
		};

		unsigned int indicies[] = {
			0, 1, 2,
			2, 3, 0
		};
		
		m_va = std::make_unique<VertexArray>();
		//size is how big the positions array is, 4 * 3 * sizeof(float) because the positions is 4 vertacies with an x, y, z component
		m_vb = std::make_unique<VertexBuffer>(positions, 4 * 6 * sizeof(float)); 


		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		m_va->AddBuffer(*m_vb, layout);

		m_ib = std::make_unique<IndexBuffer>(indicies, 6);

		m_shader = std::make_unique<Shader>("res/shaders/BasicColor.shader");
		m_shader->Bind();

		m_shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.5f, 1.0f);
		m_shader->SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
		m_shader->SetUniform3f("u_LightPos", 0.0f, 0.0f, 0.0f);
	}
	Test3D::~Test3D()
	{}

	void Test3D::OnUpdate(float deltaTime)
	{}

	void Test3D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer render;
		render.Clear();


		//glm::mat4 model = glm::translate(glm::mat4(1), m_transform);

		//glm::mat4 rotz = glm::rotate(model, (m_rotation.z/180.0f) * 3.1415f , glm::vec3(0.0f, 0.0f, 1.0f));
		//glm::mat4 roty = glm::rotate(model, (m_rotation.y/180.0f) * 3.1415f , glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::mat4 rotx = glm::rotate(model, (m_rotation.x/180.0f) * 3.1415f , glm::vec3(1.0f, 0.0f, 0.0f));

		//glm::mat4 model = glm::rotate(glm::translate(glm::mat4(1), m_transform), rotz, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = rotx * roty * rotz;
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_transform);
		model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		
		glm::mat4 mvp = m_proj * m_view * model;

		m_shader->Bind();
		m_shader->SetUniformMat4f("u_MVP", mvp);

		render.Draw(*m_va, *m_ib, *m_shader);
	}

	void Test3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &m_transform.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("Rotation", &m_rotation.x, -180, 180);
	}
}