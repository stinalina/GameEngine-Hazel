#include <Hazel.h>

#include "imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer():
		Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		// Generate vertices and buffer
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};
		m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			Hazel::BufferLayout layout = {
				{ Hazel::ShaderDataType::Float3, "a_Position"},
				{ Hazel::ShaderDataType::Float4, "a_Color"}
			};
			m_VertexBuffer->SetLayout(layout);
		}

		// Generate indices and buffer
		uint32_t indices[3] = {
			0, 1, 2
		};
		m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Links the vertexBuffer with its layout 
		m_VertexArray.reset(Hazel::VertexArray::Create());
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position; // a for attribut for incoming
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position; // v for var between the shaders; this goes out
			out vec4 v_Color;		

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position; // here the var from vertexShader comes in
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));

		// Start test with making an additional square
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		std::shared_ptr<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position"}
			});
		{
			Hazel::BufferLayout layout = {
				{ Hazel::ShaderDataType::Float3, "a_Position"},
				{ Hazel::ShaderDataType::Float4, "a_Color"}
			};
			m_VertexBuffer->SetLayout(layout);
		}

		// Generate indices and buffer
		uint32_t squareIndices[6] = {
			0, 1, 2, 2, 3, 0
		};
		std::shared_ptr<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		// Links the vertexBuffer with its layout 
		m_SquareVA.reset(Hazel::VertexArray::Create());
		m_SquareVA->AddVertexBuffer(squareVB);
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc2 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position; 
			//layout(location = 1) in vec4 a_Color;

			out vec3 v_Position; 
			//out vec4 v_Color;	

			uniform mat4 u_ViewProjection;	

			void main()
			{
				v_Position = a_Position;
				//v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc2 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position; 

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_Shader2.reset(new Hazel::Shader(vertexSrc2, fragmentSrc2));
	}

	void OnUpdate() override
	{
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0 });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition({ 0.5f, 0.5f, 0.f });
		m_Camera.SetRotation(45.0f);

		Hazel::Renderer::BeginScene(m_Camera);
		{
			Hazel::Renderer::Submit(m_Shader2, m_SquareVA);
			Hazel::Renderer::Submit(m_Shader, m_VertexArray);
			Hazel::Renderer::EndScene();
		}
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(Hazel::Event& event) override
	{
		
	}

private:
	std::shared_ptr<Hazel::Shader> m_Shader;
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> m_IndexBuffer;

	std::shared_ptr<Hazel::Shader> m_Shader2;
	std::shared_ptr<Hazel::VertexArray> m_SquareVA;

	Hazel::OrthographicCamera m_Camera;
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox(){}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}