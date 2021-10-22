#include "TestTexture.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

#include "imgui/imgui.h"

test::TestTexture::TestTexture() : m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
									m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
									m_transform(glm::vec3(200.0f, 0.0f, 0.0f))
{
	float positions[] =
	{
		-50.0f, -50.0f, 0.0f, 0.0f,
		100.0f, -50.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 1.0f, 1.0f,
		-50.0f, 100.0f, 0.0f, 1.0f
	};

	unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	m_va = std::make_unique<VertexArray>();
	m_vb = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);
	m_va->AddBuffer(*m_vb, layout);

	m_ib = std::make_unique<IndexBuffer>(indicies, 6);

	m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");
	m_shader->Bind();

	m_texture = std::make_unique<Texture>("res/textures/image.png");

	m_shader->SetUniform1i("u_Texture", 0);
}

test::TestTexture::~TestTexture()
{
}

void test::TestTexture::OnUpdate(float deltaTime)
{
}

void test::TestTexture::OnRender()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	Renderer render;

	m_texture->Bind();

	render.Clear();

	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_transform);
	glm::mat4 mvp = m_proj * m_view * model;

	m_shader->Bind();
	m_shader->SetUniformMat4f("u_MVP", mvp);

	render.Draw(*m_va, *m_ib, *m_shader);
}

void test::TestTexture::OnImGuiRender()
{
	ImGui::SliderFloat3("Translation", &m_transform.x, 0.0f, 960.0f);
}
