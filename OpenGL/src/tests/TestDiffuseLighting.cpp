#include "TestDiffuseLighting.h"

#include "VertexBufferLayout.h"

#include "glm/gtc/matrix_transform.hpp"
#include "vendor/imgui/imgui.h"

namespace test
{
	TestDiffuseLighting::TestDiffuseLighting() 
		: m_Proj(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f)),
		m_View(glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.25f, -3.0f)), 
							glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
		m_ModelRotation(0.0f, 0.0f, 0.0f),
		m_LightPos(1.0f, 1.0f, 1.0f)
	{
		// position - uv - normal
		float verticies[] = {
			// front face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f, // 0
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f, // 1
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f, // 2
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f, // 3
			// right face		   		 	  
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f,  0.0f,  0.0f, // 4
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f, // 5
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f, // 6
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f, // 7
			// back face		   		 	  
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f,  1.0f, // 9
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f,  1.0f, // 8
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f,  1.0f, // 12
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f,  1.0f, // 11
			// left face		 		 	  
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // 12
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // 13
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // 14
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // 15
			// top face			 			  
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f, // 16
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f, // 17
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f, // 18
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f, // 19
			// bottom face		 			  
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // 20
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f, // 21
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f, // 22
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f, // 23
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
		// 24 vecticies * 6 dimensions
		m_VBO = std::make_unique<VertexBuffer>(verticies, 24 * (3 + 2 + 3) * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3); // position
		layout.Push<float>(2); // UV coords
		layout.Push<float>(3); // normal
		m_VAO->AddBuffer(*m_VBO, layout);

		// 6 indices per square * 6 faces
		m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6 * 6);

		// Initialize shader
		m_Shader = std::make_unique<Shader>("res/shader/Diffuse.shader");
		m_Shader->Bind();

		// Initialize texture
		m_Texture = std::make_unique<Texture>("res/textures/pop-me.png");
		m_Shader->SetUniform1i("u_Texture", 0);

		// Light shader
		m_LightShader = std::make_unique<Shader>("res/shader/UnlitColor.shader");
		m_LightShader->Bind();
		m_LightShader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	}

	TestDiffuseLighting::~TestDiffuseLighting()
	{
	}

	void TestDiffuseLighting::OnUpdate(float deltaTime)
	{
	}

	void TestDiffuseLighting::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		// Render cube
		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians(m_ModelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_ModelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(m_ModelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Proj", m_Proj);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Model", model);
		m_Shader->SetUniform3f("u_LightPos", m_LightPos.x, m_LightPos.y, m_LightPos.z);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

		// Render light source
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		m_LightShader->Bind();
		m_LightShader->SetUniformMat4f("u_MVP", m_Proj * m_View * model);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_LightShader);
	}

	void TestDiffuseLighting::OnImGuiRender()
	{
		ImGui::SliderFloat3("Rotation", &m_ModelRotation.x, 0.0f, 360.0f, "%.2f deg");
		ImGui::SliderFloat3("Light Pos", &m_LightPos.x, -2.0f, 2.0f);
	}
}