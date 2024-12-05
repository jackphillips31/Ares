#include "assets/shaders/FlatColorShaderSource.h"

#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow()), myFont(nullptr)
{
	Ares::BufferLayout bufferLayout = Ares::BufferLayout({
		{ Ares::ShaderDataType::Float3, "a_Position"},
		{ Ares::ShaderDataType::Float4, "a_Color"}
	});

	float vertices[28] = {
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	};

	uint32_t indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	m_VBO = Ares::VertexBuffer::Create(vertices, sizeof(vertices));
	m_VBO->SetLayout(bufferLayout);

	m_IBO = Ares::IndexBuffer::Create(6);
	m_IBO->SetData(indices, 6);

	m_VAO = Ares::VertexArray::Create();
	m_VAO->AddVertexBuffer(m_VBO);
	m_VAO->SetIndexBuffer(m_IBO);

	Ares::FileBuffer shaderFile = Ares::FileIO::LoadFile("assets/shaders/FlatColorShader.shader");
	std::string shaderSource(static_cast<const char*>(shaderFile.GetBuffer()), shaderFile.GetSize());
	m_Shader = Ares::Shader::Create("FlatColorShader", shaderSource);
	m_Shader->Bind();
}

void Sandbox2D::OnAttach()
{
	//const char* fontPath = "assets/fonts/Inter_18pt-SemiBold.ttf";
	const char* fontPath = "assets/fonts/Inter_18pt-Regular.ttf";
	float fontSize = 14.0f;

	ImFontConfig config;
	config.PixelSnapH = true;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontGlobalScale = 1.0f;
	io.Fonts->AddFontDefault();
	myFont = io.Fonts->AddFontFromFileTTF(fontPath, fontSize, &config);
	io.Fonts->Build();
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Ares::Timestep ts)
{
	m_FrameBufferElement.OnUpdate();
}

void Sandbox2D::OnRender()
{
	Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Ares::RenderCommand::Clear();

	Ares::Ref<Ares::FrameBuffer> frameBuffer = m_FrameBufferElement.GetFrameBuffer();
	ImVec2 availableSize = m_FrameBufferElement.GetContentRegionAvail();

	if (frameBuffer)
	{
		frameBuffer->Bind();

		Ares::RenderCommand::SetViewport(0, 0, static_cast<uint32_t>(availableSize.x), static_cast<uint32_t>(availableSize.y));
		Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Ares::RenderCommand::Clear();

		Ares::RenderCommand::DrawIndexed(m_VAO);
		frameBuffer->Unbind();
	}
}

void Sandbox2D::OnEvent(Ares::Event& e)
{
	m_WindowSettingsElement.OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::PushFont(myFont);

	m_MainWindowElement.Draw();

	m_PerformanceElement.Draw();

	m_WindowSettingsElement.Draw();

	m_FrameBufferElement.Draw();

	ImGui::ShowDemoWindow();

	Ares::Log::GetConsole()->Draw("Console", true);

	ImGui::PopFont();
}