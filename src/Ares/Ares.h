#pragma once

//------- FOR USE BY ARES APPLICATION ---------
//---------------------------------------------
#include "Engine/Core/Core.h"

#include "Engine/Debug/Log.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Flags.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/ThreadPool.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Data/Asset.h"
#include "Engine/Data/AssetManager.h"
#include "Engine/Data/FileBuffer.h"
#include "Engine/Data/FileIO.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/EventQueue.h"
#include "Engine/Events/KeyCodes.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseCodes.h"
#include "Engine/Events/MouseEvent.h"

#include "Engine/ImGui/ImGuiElement.h"
#include "Engine/Layers/ImGuiLayer.h"

#include "Engine/Utility/File.h"

//----------------- RENDERER ------------------
//---------------------------------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/RenderCommand.h"

#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/FrameBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/Scene/OrthographicCamera.h"
#include "Engine/Renderer/Scene/PerspectiveCamera.h"