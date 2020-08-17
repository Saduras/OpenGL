#pragma once
#include "Test.h"

#include <VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <Shader.h>
#include <Texture.h>

namespace test
{
	class TestUnlit3DCube : public Test
	{
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj;
		glm::mat4 m_View;
		glm::mat4 m_Model;
	public:
		TestUnlit3DCube();
		~TestUnlit3DCube();

		void OnRender() override;
		void OnImGuiRender() override;
	};
}

