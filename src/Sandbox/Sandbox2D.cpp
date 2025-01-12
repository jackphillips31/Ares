#include "Sandbox2D.h"

#include "assets/resource.h"
#include "utility/WinLoader.h"

const uint32_t g_DefaultWhiteTexture = 0xffffffff;

using namespace Ares::ECS;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	m_Window(Ares::Application::Get().GetWindow()), myFont(nullptr)
{
	Ares::EventQueue::AddListener<Ares::WindowFocusEvent>(AR_BIND_EVENT_FN(Sandbox2D::OnWindowFocus));

	m_SandboxScene = Ares::CreateScope<Scene>();
	m_SandboxScene->RegisterSystem<Systems::CameraSystem>();
	m_SandboxScene->RegisterSystem<Systems::RenderSystem>();
	m_SandboxScene->RegisterSystem<Systems::LightSystem>();
	m_SandboxScene->SetSystemUpdateOrder<Systems::CameraSystem, Systems::LightSystem, Systems::RenderSystem>();
	m_SandboxScene->SetSystemRenderOrder<Systems::RenderSystem>();

	m_EntityListElement.SetScene(m_SandboxScene.get());

	m_SandboxScene->Init();
}

void Sandbox2D::OnAttach()
{
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.FontGlobalScale = 1.0f;

	// Load ImGui Config from resources
	DWORD configSize = 0;
	void* configData = LoadFromResource(MAKEINTRESOURCE(IDR_IMGUICONFIG), configSize);

	if (configData && configSize > 0)
	{
		ImGui::LoadIniSettingsFromMemory(static_cast<const char*>(configData), static_cast<size_t>(configSize));
	}

	// Load Inter_18pt-Regular font from resources
	DWORD regularFontSize = 0;
	void* regularFontData = LoadFromResource(MAKEINTRESOURCE(IDR_FONT2), regularFontSize);

	if (regularFontData && regularFontSize > 0)
	{
		ImFontConfig config;
		config.PixelSnapH = true;
		config.FontDataOwnedByAtlas = false;

		myFont = io.Fonts->AddFontFromMemoryTTF(regularFontData, regularFontSize, 14.0f, &config);
	}

	// Load MaterialIcons-Regular font from resource
	DWORD iconFontSize = 0;
	void* iconFontData = LoadFromResource(MAKEINTRESOURCE(IDR_FONT1), iconFontSize);

	if (iconFontData && iconFontSize > 0)
	{
		// Unicode range for Material Icons
		static const ImWchar icon_ranges[] = { 0xE000, 0xEFFF, 0 };
		ImFontConfig iconConfig;

		// Merge with the regular font
		iconConfig.MergeMode = true;
		iconConfig.PixelSnapH = true;
		iconConfig.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF(iconFontData, iconFontSize, 14.0f, &iconConfig, icon_ranges);
	}

	// Build the font atlas
	io.Fonts->Build();

	CreateLightEntity();
	CreateCameraEntity();
	CreateQuadMeshEntities();
	CreateTeapotEntity();
	LoadShaderProgram();
	LoadDefaultTexture();

	Ares::RenderCommand::SetFaceCulling(true);
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(const Ares::Timestep& ts)
{
	m_FrameBufferElement.OnUpdate(ts);

	m_EntityListElement.OnUpdate(ts);

	m_SandboxScene->OnUpdate(ts);
}

void Sandbox2D::OnRender()
{
	Ares::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0.1f });
	Ares::RenderCommand::Clear();

	Ares::FrameBuffer* frameBuffer = m_FrameBufferElement.GetFrameBuffer();
	ImVec2 availableSize = m_FrameBufferElement.GetContentRegionAvail();

	Systems::CameraSystem* camera = m_SandboxScene->GetSystem<Systems::CameraSystem>();

	if (availableSize.x && availableSize.y)
	{
		camera->SetViewportSize({ availableSize.x, availableSize.y });
	}

	if (frameBuffer)
	{
		frameBuffer->Bind();
		m_SandboxScene->OnRender();
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

	m_AssetListElement.Draw();

	m_EntityListElement.Draw();

	ImGui::ShowDemoWindow();

	Ares::Log::GetConsole()->Draw("Console", true);

	ImGui::PopFont();
}

bool Sandbox2D::OnWindowFocus(Ares::WindowFocusEvent& event)
{
	AR_TRACE(event);
	return false;
}

void Sandbox2D::CreateLightEntity()
{
	EntityManager* entityManager = m_SandboxScene->GetEntityManager();

	m_LightEntity = entityManager->CreateEntity();
	Components::Light* lightComponent = m_LightEntity.AddComponent<Components::Light>();
	Components::Transform* lightTransform = m_LightEntity.AddComponent<Components::Transform>();
	lightComponent->SetColor({ 1.0f, 1.0f, 1.0f });
	lightComponent->SetType(Components::Light::Point);
	lightComponent->SetRange(10.0f);
	lightComponent->SetFalloff(5.0f);
	lightComponent->SetInnerAngle(45.0f);
	lightComponent->SetOuterAngle(90.0f);
	lightComponent->SetIntensity(3.0f);
	lightTransform->SetPosition({ -5.0f, 5.0f, 2.0f });
	m_LightEntity.SetName("Light Entity");
}

void Sandbox2D::CreateCameraEntity()
{
	EntityManager* entityManager = m_SandboxScene->GetEntityManager();

	m_CameraEntity = entityManager->CreateEntity();
	m_CameraEntity.AddComponent<Components::Camera>(Components::Camera::Perspective);
	Components::Transform* camTransform = m_CameraEntity.AddComponent<Components::Transform>();
	camTransform->SetPosition({ 0.0f, 0.0f, 2.0f });
	camTransform->SetRotation({ 0.0f, 0.0f, 0.0f });
	m_CameraEntity.SetName("Camera Entity");

	m_SandboxScene->GetSystem<Systems::CameraSystem>()->SetActiveCamera(m_CameraEntity);
}

void Sandbox2D::CreateQuadMeshEntities()
{
	// Load Quad Mesh
	DWORD quadMeshSize = 0;
	void* quadMeshData = LoadFromResource(MAKEINTRESOURCE(IDR_QUADMESH), quadMeshSize);

	if (quadMeshData && quadMeshSize > 0)
	{
		Ares::MemoryDataKey meshKey = Ares::MemoryDataProvider::RegisterData(quadMeshData, static_cast<size_t>(quadMeshSize));
		Ares::Ref<Ares::Asset> quadMeshAsset = Ares::AssetManager::Stage<Ares::MeshData>("QuadMesh", meshKey);

		EntityManager* entityManager = m_SandboxScene->GetEntityManager();

		m_SquareEntity = entityManager->CreateEntity();
		m_SquareEntity.AddComponent<Components::Mesh>(quadMeshAsset);
		Components::Material* quad1Material = m_SquareEntity.AddComponent<Components::Material>();
		Components::Transform* quad1Transform = m_SquareEntity.AddComponent<Components::Transform>();
		Components::MaterialProperties props;
		props.Basic.Color = { 1.0f, 1.0f, 0.0f };
		quad1Material->SetProperties(props);
		quad1Transform->SetPosition({ 0.0f, 0.0f, 0.0f });
		quad1Transform->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_SquareEntity.SetName("SquareMesh");

		m_SquareEntity2 = entityManager->CreateEntity();
		m_SquareEntity2.AddComponent<Components::Mesh>(quadMeshAsset);
		Components::Material* quad2Material = m_SquareEntity2.AddComponent<Components::Material>();
		Components::Transform* quad2Transform = m_SquareEntity2.AddComponent<Components::Transform>();
		Components::MaterialProperties props2;
		props2.Basic.Color = { 1.0f, 0.0f, 0.0f };
		props2.Basic.Alpha = 1.0f;
		props2.Surface.Roughness = 0.9f;
		props2.Surface.Metallic = 0.9f;
		quad2Material->SetProperties(props2);
		quad2Transform->SetPosition({ 1.0f, 1.0f, 0.5f });
		quad2Transform->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_SquareEntity2.SetName("SquareMesh2");

		m_SquareEntity3 = entityManager->CreateEntity();
		m_SquareEntity3.AddComponent<Components::Mesh>(quadMeshAsset);
		m_SquareEntity3.AddComponent<Components::Material>();
		Components::Transform* quad3Transform = m_SquareEntity3.AddComponent<Components::Transform>();
		quad3Transform->SetPosition({ -1.0f, 0.0f, 0.0f });
		quad3Transform->SetRotation({ 0.0f, 0.0f, 0.0f });
		m_SquareEntity3.SetName("SquareMesh3");

		Ares::AssetManager::Load(quadMeshAsset);
	}
}

void Sandbox2D::CreateTeapotEntity()
{
	// Load Teapot Mesh
	DWORD teapotMeshSize = 0;
	void* teapotMeshData = LoadFromResource(MAKEINTRESOURCE(IDR_TEAPOTMESH), teapotMeshSize);

	if (teapotMeshData && teapotMeshSize > 0)
	{
		Ares::MemoryDataKey teapotDataKey = Ares::MemoryDataProvider::RegisterData(teapotMeshData, static_cast<size_t>(teapotMeshSize));
		Ares::Ref<Ares::Asset> teapotMeshAsset = Ares::AssetManager::Stage<Ares::MeshData>("TeapotMesh", teapotDataKey);

		EntityManager* entityManager = m_SandboxScene->GetEntityManager();

		m_TeapotEntity = entityManager->CreateEntity();
		m_TeapotEntity.AddComponent<Components::Mesh>(teapotMeshAsset);
		m_TeapotEntity.AddComponent<Components::Material>();
		Components::Transform* teapotTransform = m_TeapotEntity.AddComponent<Components::Transform>();
		teapotTransform->SetPosition({ 0.0f, 0.0f, 0.0f });
		teapotTransform->SetRotation({ 0.0f, 0.0f, 0.0f });
		teapotTransform->SetScale({ 0.5f, 0.5f, 0.5f });
		m_TeapotEntity.SetName("Teapot");

		Ares::AssetManager::Load(teapotMeshAsset);
	}
}

void Sandbox2D::LoadShaderProgram()
{
	// Load Shader Program
	
	DWORD shaderSize = 0;
	void* shaderData = LoadFromResource(MAKEINTRESOURCE(IDR_TEXTURESHADER), shaderSize);

	/*
	Ares::DataBuffer vertexShaderData = Ares::FileIO::LoadFile("assets/shaders/TextureShaderVertex.glsl");
	Ares::DataBuffer fragmentShaderData = Ares::FileIO::LoadFile("assets/shaders/TextureShaderFragment.glsl");
	Ares::MemoryDataKey vertKey = Ares::MemoryDataProvider::RegisterData(std::move(vertexShaderData));
	Ares::MemoryDataKey fragKey = Ares::MemoryDataProvider::RegisterData(std::move(fragmentShaderData));
	Ares::Ref<Ares::Asset> vertexShader = Ares::AssetManager::Stage<Ares::VertexShader>("VertexShader", "assets/shaders/MaterialVertex.glsl");
	Ares::Ref<Ares::Asset> fragmentShader = Ares::AssetManager::Stage<Ares::FragmentShader>("FragmentShader", "assets/shaders/MaterialFragment.glsl");
	*/

	if (shaderData && shaderSize > 0)
	{
		Ares::MemoryDataKey shaderKey = Ares::MemoryDataProvider::RegisterData(shaderData, static_cast<size_t>(shaderSize));
		Ares::Ref<Ares::Asset> shaderProgram = Ares::AssetManager::Stage<Ares::ShaderProgram>("ShaderProgram", shaderKey);

		Ares::AssetManager::Load(shaderProgram,
			[SE1 = m_SquareEntity, SE2 = m_SquareEntity2, SE3 = m_SquareEntity3, TE1 = m_TeapotEntity, scene = m_SandboxScene.get()](const Ares::Ref<Ares::Asset>& asset) {
				if (asset->GetState() == Ares::AssetState::Loaded)
				{
					EntityManager* entityManager = scene->GetEntityManager();
					Components::Material* material1 = entityManager->GetComponent<Components::Material>(SE1);
					Components::Material* material2 = entityManager->GetComponent<Components::Material>(SE2);
					Components::Material* material3 = entityManager->GetComponent<Components::Material>(SE3);
					Components::Material* material4 = entityManager->GetComponent<Components::Material>(TE1);
					material1->SetShader(asset);
					material2->SetShader(asset);
					material3->SetShader(asset);
					material4->SetShader(asset);
				}
			});
	}
}

void Sandbox2D::LoadDefaultTexture()
{
	// Load default Texture
	DWORD textureSize = 0;
	void* textureData = LoadFromResource(MAKEINTRESOURCE(IDR_DEFAULTTEXTURE), textureSize);

	// Default white texture
	const uint32_t defaultWhite = 0xffffffff;

	if (textureData && textureSize > 0)
	{
		Ares::MemoryDataKey textureKey = Ares::MemoryDataProvider::RegisterData(textureData, static_cast<size_t>(textureSize));
		Ares::MemoryDataKey whiteTextureKey = Ares::MemoryDataProvider::RegisterData(&defaultWhite, sizeof(uint32_t));
		Ares::Ref<Ares::Asset> defaultTexture = Ares::AssetManager::Stage<Ares::Texture>("DefaultTexture", textureKey);
		Ares::Ref<Ares::Asset> defaultWhiteTexture = Ares::AssetManager::Stage<Ares::Texture>("DefaultWhite", whiteTextureKey);

		Ares::AssetManager::Load({defaultTexture, defaultWhiteTexture},
			[SE1 = m_SquareEntity, SE2 = m_SquareEntity2, SE3 = m_SquareEntity3, TE1 = m_TeapotEntity, scene = m_SandboxScene.get()](const Ares::Ref<Ares::Asset>& asset) {
				if (asset->GetState() == Ares::AssetState::Loaded)
				{
					EntityManager* entityManager = scene->GetEntityManager();
					if (asset->GetName() == "DefaultTexture")
					{
						Components::Material* material1 = entityManager->GetComponent<Components::Material>(SE1);
						Components::Material* material2 = entityManager->GetComponent<Components::Material>(SE2);
						Components::Material* material3 = entityManager->GetComponent<Components::Material>(SE3);
						material1->SetTexture("u_DefaultTexture", asset);
						material2->SetTexture("u_DefaultTexture", asset);
						material3->SetTexture("u_DefaultTexture", asset);
					}
					else if (asset->GetName() == "DefaultWhite")
					{
						Components::Material* material = entityManager->GetComponent<Components::Material>(TE1);
						material->SetTexture("u_DefaultTexture", asset);
					}
					
				}
			});
	}
}