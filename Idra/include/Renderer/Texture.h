#pragma once

#include "Core/Core.h"

#include <filesystem>

namespace Idra {
	using Path = std::filesystem::path;

	class IDRA_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};

	class IDRA_API Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const Path& path, bool flipImage = true);
	};

	class IDRA_API TextureCube : public Texture
	{
	public:
		static Ref<TextureCube> Create(const std::vector<Path>& paths, bool flipImage = true);
	};

	// @TODO: make a Texture3D & TextureCube class
}