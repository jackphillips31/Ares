#pragma once
#include <glm/vec2.hpp>

#include "Engine/Data/Asset.h"

namespace Ares {

	struct RawData;

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
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual Format GetFormat() const = 0;

		// Binding and state
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;
		virtual bool IsBound() const = 0;
		virtual uint32_t GetBoundSlot() const = 0;

		// Settings
		virtual void SetFilter(const Filter minFilter, const Filter maxFilter) = 0;
		virtual void SetWrap(const Wrap wrap) = 0;

		// Utilities
		virtual void Resize(const uint32_t width, const uint32_t height) = 0;
		virtual void SetData(const RawData& data) = 0;
		virtual void GenerateMips() = 0;
		virtual bool IsValid() const = 0;

		// RendererID access (for low-level operations)
		virtual uint32_t GetRendererID() const = 0;

		// Equality operator
		inline bool operator==(const Texture& other) const
		{
			return GetRendererID() == other.GetRendererID();
		}

	private:
		// Only Asset Manager can create Textures
		friend class AssetManager;
		static Scope<Texture> Create(const std::string& name, const RawData& data);
		static Scope<Texture> Create(const std::string& name, const glm::uvec2& dimensions, const RawData& rawData, const Format format = Format::RGBA);
	};

}