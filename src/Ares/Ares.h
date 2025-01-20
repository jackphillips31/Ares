/**
 * @file Ares.h
 * 
 * @brief Main header file for the Ares engine.
 * 
 * @details This file includes core headers for the Ares engine, providing access
 * to various subsystems such as the engine core, data management, ECS (Entity
 * Component System), event handling, and rendering systems. It serves as a
 * central point for including the most commonly used modules and components
 * required for building and running an application with Ares.
 * 
 * The headers are organized into logical sections to group related functionality:
 * - Core Engine Components
 * - Data Management and Asset Handling
 * - ECS (Entity Component System)
 * - Event Handling
 * - ImGui Integration
 * - Renderer
 * 
 * @note This file is intended for use by the Ares application and provides the
 * necessary includes for interacting with the Ares game engine.
 * 
 * @section core Core Engine Components
 * - Core.h: Essential engine functions and types.
 * - Application.h: Base application class.
 * - Flags.h: Flags for configuration and settings.
 * - Input.h: Input handling.
 * - Layer.h: Base layer class.
 * - MainThreadQueue.h: Main thread task queue for renderer commands and other tasks.
 * - ThreadPool.h: Multi-threading support for background tasks.
 * - Timestep.h: Time step calculations.
 * - Utility.h: Utility functions.
 * - Window.h: Base window class.
 * 
 * @section data Data Management and Asset Handling
 * - Asset.h: Base asset class and data structure.
 * - AssetManager.h: Asset management system for loading and caching assets.
 * - DataBuffer.h: Data buffer utilities for managing raw data.
 * - MemoryDataProvider.h: Data provider that fetches data from memory.
 * - RawData.h: Raw data structure.
 * 
 * @section ecs Entity Component System (ECS)
 * - Component.h: Base class for ECS components.
 * - Entity.h: Entity class representing game objects.
 * - EntityManager.h: Manages entities and component storage.
 * - Scene.h: Represents a scene containing entities and systems.
 * - System.h: Base class for ECS systems:
 * - AllComponents.h: Includes all ECS component headers.
 * - CameraSystem.h: ECS system handling camera components.
 * - LightSystem.h: ECS system for managing light components.
 * - RenderSystem.h: ECS system responsible for rendering entities.
 * 
 * @section events Event Handling
 * - ApplicationEvent.h: Events related to the application lifecycle.
 * - AssetEvent.h: Events triggered by asset loading and management.
 * - Event.h: Base event class for custom event types.
 * - EventQueue.h: Event queue for managing events in the application.
 * - KeyCodes.h: Key codes for keyboard input events.
 * - KeyEvent.h: Key event classes.
 * - MouseCodes.h: Mouse input codes.
 * - MouseEvent.h Mouse event classes.
 * 
 * @section imgui ImGui Integration
 * - ImGuiElement.h: Base class for custom ImGui elements.
 * - ImGuiLayer.h: Layer for integrating ImGui into the Ares engine.
 * 
 * @section renderer Renderer
 * - Renderer.h: Main rendering interface and setup.
 * - RenderCommand.h: Class with methods for rendering operations.
 * - MeshData.h: Data structures for mesh assets.
 * - Shader.h: Shader class for managing and applying shaders.
 * - Texture.h: Texture handling and loading.
 * - Buffer.h: Buffer management for GPU data.
 * - BufferLayout.h: Defines buffer layouts for vertex data.
 * - FrameBuffer.h: Framebuffer management for offscreen rendering.
 * - UniformBuffer.h: Uniform buffer handling for shader data.
 * - VertexArray.h: Vertex array object management for rendering.h
 */
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