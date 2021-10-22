#pragma once
#include "Test.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test
{
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_va;
		std::unique_ptr<IndexBuffer> m_ib;
		std::unique_ptr<VertexBuffer> m_vb;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<Texture> m_texture;

		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::vec3 m_transform;
	};
}
