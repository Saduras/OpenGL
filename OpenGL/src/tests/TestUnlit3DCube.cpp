#include "TestUnlit3DCube.h"

#include "VertexBufferLayout.h"

#include "glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace test
{
	TestUnlit3DCube::TestUnlit3DCube()
		: m_Proj(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f)),
		m_View(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.25f, -3.0f)),
			glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
		m_ModelAngle(0.0f)
	{
		float positions[] = {
			-0.5f, -0.5f, -0.5f, // 0
			 0.5f, -0.5f, -0.5f, // 1
			 0.5f,  0.5f, -0.5f, // 2
			-0.5f,  0.5f, -0.5f, // 3
			-0.5f, -0.5f,  0.5f, // 4
			 0.5f, -0.5f,  0.5f, // 5
			 0.5f,  0.5f,  0.5f, // 6
			-0.5f,  0.5f,  0.5f, // 7
		};

		unsigned int indicies[] = {
			// front face
			0, 1, 2,
			2, 3, 0,
			// right face
			1, 5, 6,
			6, 2, 1,
			// back face
			5, 7, 4,
			7, 6, 5,
			// left face
			4, 3, 0,
			3, 7, 4,
			// top face
			3, 2, 6,
			6, 7, 3,
			// bottom face
			0, 1, 5,
			5, 4, 0, 
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Initalize vertex array
		m_VAO = std::make_unique<VertexArray>();
		// 8 vecticies * 3 dimensions
		m_VBO = std::make_unique<VertexBuffer>(positions, 8 * 3 * sizeof(float));
		VertexBufferLayout layout;
		// 3 dimensions
		layout.Push<float>(3);
		m_VAO->AddBuffer(*m_VBO, layout);

		// 6 indices per square * 6 faces
		m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6 * 6);

		// Initialize shader
		m_Shader = std::make_unique<Shader>("res/shader/UnlitColor.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
	}

	TestUnlit3DCube::~TestUnlit3DCube()
	{
	}

	void TestUnlit3DCube::OnUpdate(float deltaTime)
	{
		m_ModelAngle += 180.0f * deltaTime;
	}
		
	void TestUnlit3DCube::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(m_ModelAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = m_Proj * m_View * model;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestUnlit3DCube::OnImGuiRender()
	{
	}
}