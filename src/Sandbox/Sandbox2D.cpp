#include "Sandbox2D.h"

const uint32_t g_DefaultWhiteTexture = 0xffffffff;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow()), myFont(nullptr)
{
	Ares::BufferLayout bufferLayout = Ares::BufferLayout({
		{ Ares::ShaderDataType::Float3, "a_Position"},
		{ Ares::ShaderDataType::Float4, "a_Color"},
		{ Ares::ShaderDataType::Float2, "a_TexCoord"},
		{ Ares::ShaderDataType::Float, "a_TexIndex"},
		{ Ares::ShaderDataType::Float, "a_TilingFactor"}
		});

	float vertices[110] = {
		// SQUARE
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,	// 0 - Top left
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// 1 - Top right
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// 2 - Bottom left
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// 3 - Bottom right

		// RED -X
		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 4 - Bottom right
		-1.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 5 - Top
		-2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 6 - Bottom left

		// GREEN +X
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 7 - Bottom left
		1.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// 8 - Top
		2.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f	// 9 - Bottom right
	};

	uint32_t indices[12] = {
		// SQUARE
		0, 2, 1,
		1, 2, 3,

		// RED -X
		4, 5, 6,

		// GREEN +X
		9, 8, 7
	};

	m_VBO = Ares::VertexBuffer::Create({ vertices, sizeof(vertices) });
	m_VBO->SetLayout(bufferLayout);

	m_IBO = Ares::IndexBuffer::Create({ indices, sizeof(indices) });

	m_VAO = Ares::VertexArray::Create();
	m_VAO->AddVertexBuffer(m_VBO);
	m_VAO->SetIndexBuffer(m_IBO);

	m_Camera = Ares::CreateScope<ViewportCamera>(1280.0f, 720.0f);
	m_Camera->SetPosition({ 0.0f, 0.0f, 2.0f });

	Ares::EventQueue::AddListener<Ares::WindowFocusEvent>(AR_BIND_EVENT_FN(Sandbox2D::OnWindowFocus));

	Ares::AssetManager::Load(
		Ares::AssetManager::Stage<Ares::ShaderProgram>("TextureShader", "assets/shaders/TextureShaderSource.shader"),
		[this](Ares::Ref<Ares::Asset> asset)
		{
			int32_t samplers[4];
			for (uint32_t i = 0; i < 4; i++)
			{
				samplers[i] = i;
			}

			m_ShaderProgram = asset->GetAsset<Ares::ShaderProgram>();
			m_ShaderProgram->Bind();
			m_ShaderProgram->SetIntArray("u_Textures", samplers, 4);
		}
	);

	Ares::AssetManager::Load(
		{
			Ares::AssetManager::Stage<Ares::Texture>("DefaultTexture", "assets/textures/DefaultTexture.png"),
			Ares::AssetManager::Stage<Ares::Texture>("DefaultWhite", { &g_DefaultWhiteTexture, sizeof(uint32_t) })
		},
		[this](Ares::Ref<Ares::Asset> asset) {
			if (asset->GetName() == "DefaultTexture")
				m_DefaultTexture = asset->GetAsset<Ares::Texture>();
			if (asset->GetName() == "DefaultWhite")
				m_DefaultWhite = asset->GetAsset<Ares::Texture>();
		}
	);
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
	m_Camera->OnUpdate(ts);
}

void Sandbox2D::OnRender()
{
	Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Ares::RenderCommand::Clear();

	Ares::Ref<Ares::FrameBuffer> frameBuffer = m_FrameBufferElement.GetFrameBuffer();
	ImVec2 availableSize = m_FrameBufferElement.GetContentRegionAvail();

	if (availableSize.x && availableSize.y)
		m_Camera->SetViewportSize({ availableSize.x, availableSize.y });

	if (frameBuffer && m_ShaderProgram && m_DefaultTexture && m_DefaultWhite)
	{
		m_ShaderProgram->Bind();
		m_ShaderProgram->SetMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix());

		m_DefaultWhite->Bind(0);
		m_DefaultTexture->Bind(1);

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
	m_Camera->OnEvent(e);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::PushFont(myFont);

	m_MainWindowElement.Draw();

	m_PerformanceElement.Draw();

	m_WindowSettingsElement.Draw();

	m_FrameBufferElement.Draw();

	m_AssetListElement.Draw();

	ImGui::ShowDemoWindow();

	Ares::Log::GetConsole()->Draw("Console", true);

	ImGui::PopFont();
}

bool Sandbox2D::OnWindowFocus(Ares::WindowFocusEvent& event)
{
	AR_TRACE(event);
	return false;
}