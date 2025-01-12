#pragma once

//------- FOR USE BY ARES APPLICATION ---------
//---------------------------------------------
#include "Engine/Core/Core.h"

#include "Engine/Debug/Log.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Flags.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/MainThreadQueue.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Utility.h"
#include "Engine/Core/Window.h"

#include "Engine/Data/Asset.h"
#include "Engine/Data/AssetManager.h"
#include "Engine/Data/DataBuffer.h"
#include "Engine/Data/MemoryDataProvider.h"
#include "Engine/Data/RawData.h"

#include "Engine/ECS/Core/Component.h"
#include "Engine/ECS/Core/Entity.h"
#include "Engine/ECS/Core/EntityManager.h"
#include "Engine/ECS/Core/Scene.h"
#include "Engine/ECS/Core/System.h"
#include "Engine/ECS/Components/AllComponents.h"
#include "Engine/ECS/Systems/CameraSystem.h"
#include "Engine/ECS/Systems/LightSystem.h"
#include "Engine/ECS/Systems/RenderSystem.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/AssetEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Events/KeyCodes.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseCodes.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/ImGui/ImGuiElement.h"
#include "Engine/Layers/ImGuiLayer.h"

//----------------- RENDERER ------------------
//---------------------------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/Assets/MeshData.h"
#include "Engine/Renderer/Assets/Shader.h"
#include "Engine/Renderer/Assets/Texture.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/BufferLayout.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Renderer/VertexArray.h"