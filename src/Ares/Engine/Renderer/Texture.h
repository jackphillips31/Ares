#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Data/Asset.h"
#include "Engine/Data/FileBuffer.h"

namespace Ares {



	class Texture : public AssetBase
	{
	public:

		enum class Format : uint8_t
		{
			None = 0,
			RGB,
			RGBA,
			RGBA16F,
			RGBA32F,
			Depth24Stencil8
		};

		enum class Filter : uint8_t
		{
			None = 0,
			Linear,
			Nearest
		};

		enum class Wrap : uint8_t
		{
			None = 0,
			Repeat,
			ClampToEdge,
			ClampToBorder,
			MirroredRepeat
		};

	public:
		virtual ~Texture() = default;

		// Core properties
		virtual const std::string& GetName() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual Format GetFormat() const = 0;

		// Binding and state
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;
		virtual bool IsBound() const = 0;
		virtual uint32_t GetBoundSlot() const = 0;

		// Settings
		virtual void SetFilter(const Filter& minFilter, const Filter& maxFilter) = 0;
		virtual void SetWrap(const Wrap& wrap) = 0;

		// Utilites
		virtual void Resize(const uint32_t& width, const uint32_t& height) = 0;
		virtual void SetData(const void* data, const size_t& size) = 0;
		virtual void GenerateMips() = 0;
		virtual bool IsValid() const = 0;

		// Renderer ID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		static Ref<Texture> Create(const std::string& name, const FileBuffer& fileBuffer);
		static Ref<Texture> Create(const std::string& name, const void* data, const size_t& size);
		static Ref<Texture> Create(const std::string& name, const glm::uvec2& dimensions, Format format = Format::RGBA);

		virtual bool operator==(const Texture& other) const = 0;
	};

}