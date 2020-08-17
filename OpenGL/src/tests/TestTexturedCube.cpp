#include "TestTexturedCube.h"

#include "VertexBufferLayout.h"

#include "glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace test
{
	TestTexturedCube::TestTexturedCube()
		: m_Proj(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f)),
		m_View(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.25f, -3.0f)),
			glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
		m_ModelAngle(0.0f)
	{
		// position - uv
		float verticies[] = {
			// front face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3
			// right face		   		 	  
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 4
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 7
			// back face		   		 	  
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 9
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 8
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 12
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 11
			// left face		 		 	  
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 12
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 13
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 14
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 15
			// top face			 			  
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // 16
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // 17
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 18
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 19
			// bottom face		 			  
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 20
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 21
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // 22
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // 23
		};

		unsigned int indicies[] = {
			// front face
			0, 1, 2,
			2, 3, 0,
			// right face
			4, 5, 6,
			6, 7, 4,
			// back face
			8, 9, 10,
			10, 11, 8,
			// left face
			12, 13, 14,
			14, 15, 12,
			// top face
			16, 17, 18,
			18, 19, 16,
			// bottom face
			20, 21, 22,
			22, 23, 20,
		};

		GLCall(glEnable(GL_DEPTH_TEST));

		// Initalize vertex array
		m_VAO = std::make_unique<VertexArray>();
		// 8 vecticies * 3 dimensions
		m_VBO = std::make_unique<VertexBuffer>(verticies, 24 * (3 + 2) * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); // position
		layout.Push<float>(2); // UV coords
		m_VAO->AddBuffer(*m_VBO, layout);

		// 6 indices per square * 6 faces
		m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6 * 6);

		// Initialize shader
		m_Shader = std::make_unique<Shader>("res/shader/UnlitTexture.shader");
		m_Shader->Bind();

		// Initialize texture
		m_Texture = std::make_unique<Texture>("res/textures/pop-me.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexturedCube::~TestTexturedCube()
	{
	}

	void TestTexturedCube::OnUpdate(float deltaTime)
	{
		m_ModelAngle += 90.0f * deltaTime;
	}
		
	void TestTexturedCube::OnRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(m_ModelAngle), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = m_Proj * m_View * model;

		m_Texture->Bind();
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestTexturedCube::OnImGuiRender()
	{
	}
}