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
		//NOTE: opengl perspective axis: 
		//z+ is close to the screen, z- is away from screen
		//x+ is right of the screen, x- is left of screen
		//y+ is up, y- is down

		//NOTE: remember this array is the object itself. DO NOT TRANSFORM 
		//x, y, z, normal x, normal y, normal z
		//front, back, left, right, top, bottom
		float positions[] = {
			-25.0f, -25.0f,  25.0f, 0.0f, 0.0f, 1.0f,	//0
			 25.0f, -25.0f,  25.0f, 0.0f, 0.0f, 1.0f,
			 25.0f,  25.0f,  25.0f, 0.0f, 0.0f, 1.0f,
			-25.0f,  25.0f,  25.0f, 0.0f, 0.0f, 1.0f,	//3

			-25.0f, -25.0f, -25.0f, 0.0f, 0.0f, -1.0f,	//4
			 25.0f, -25.0f, -25.0f, 0.0f, 0.0f, -1.0f,
			 25.0f,  25.0f, -25.0f, 0.0f, 0.0f, -1.0f,
			-25.0f,  25.0f, -25.0f, 0.0f, 0.0f, -1.0f,	//7

			-25.0f, -25.0f,  25.0f, -1.0f, 0.0f, 0.0f,	//8
			-25.0f, -25.0f, -25.0f, -1.0f, 0.0f, 0.0f,
			-25.0f,  25.0f,  25.0f, -1.0f, 0.0f, 0.0f,
			-25.0f,  25.0f, -25.0f, -1.0f, 0.0f, 0.0f,	//11

			 25.0f, -25.0f,  25.0f,  1.0f, 0.0f, 0.0f,	//12
			 25.0f, -25.0f, -25.0f,  1.0f, 0.0f, 0.0f,
			 25.0f,  25.0f,  25.0f,  1.0f, 0.0f, 0.0f,
			 25.0f,  25.0f, -25.0f,  1.0f, 0.0f, 0.0f,	//15

			-25.0f,  25.0f,  25.0f,  0.0f, 1.0f, 0.0f,	//16
			 25.0f,  25.0f,  25.0f,  0.0f, 1.0f, 0.0f,
			-25.0f,  25.0f, -25.0f,  0.0f, 1.0f, 0.0f,
			 25.0f,  25.0f, -25.0f,  0.0f, 1.0f, 0.0f,	//19

			-25.0f, -25.0f,  25.0f,  0.0f, -1.0f, 0.0f,	//20
			 25.0f, -25.0f,  25.0f,  0.0f, -1.0f, 0.0f,	
			-25.0f, -25.0f, -25.0f,  0.0f, -1.0f, 0.0f,	
			 25.0f, -25.0f, -25.0f,  0.0f, -1.0f, 0.0f	//23
		};

		//culling is counter clockwise
		unsigned int indicies[] = {
			2, 1, 0,
			0, 3, 2,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			9, 11, 10,

			14, 13, 12,
			14, 15, 13,

			18, 17, 16,
			18, 19, 17,

			20, 21, 22,
			21, 23, 22
		};
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_LESS);
		glCullFace(GL_FRONT);

		m_va = std::make_unique<VertexArray>();
		//The numbers chosen is for how big the positions array is (x * y * sizeof(datatype))
		m_vb = std::make_unique<VertexBuffer>(positions, 24 * 6 * sizeof(float)); 


		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(3);
		m_va->AddBuffer(*m_vb, layout);

		m_ib = std::make_unique<IndexBuffer>(indicies, 36);

		//m_shader = std::make_unique<Shader>("res/shaders/BasicColor.shader");
		m_shader = std::make_unique<Shader>("res/shaders/BasicLighting.shader");
		m_shader->Bind();

		//m_shader->SetUniform4f("u_Color", 0.2f, 1.0f, 0.2f, 1.0f);
		m_shader->SetUniform3f("u_Color", 1.0f, 0.5f, 0.31f);
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
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer render;
		render.Clear();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_transform);
		model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		
		glm::mat4 mvp = m_proj * m_view * model;

		m_shader->Bind();
		//m_shader->SetUniformMat4f("u_MVP", mvp);
		m_shader->SetUniformMat4f("u_Model", model);
		m_shader->SetUniformMat4f("u_Projection", m_proj);
		m_shader->SetUniformMat4f("u_View", m_view);



		render.Draw(*m_va, *m_ib, *m_shader);
	}

	void Test3D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation", &m_transform.x, -500.0f, 500.0f);
		ImGui::SliderFloat3("Rotation", &m_rotation.x, -180, 180);
	}
}