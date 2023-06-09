#pragma once

#include <filesystem>
#include <plat/filesystem.hpp>
#include <plat/window.hpp>

#include "rotary/core/base.hpp"
#include "rotary/core/memory.hpp"

namespace rot
{
	class Mesh;
	class Shader;

	class ROTARY_API Graphics
	{
	public:
		virtual ~Graphics() noexcept = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		virtual Ref<Mesh> createMesh(const void* pData, size_t size) = 0;
		virtual Ref<Shader> createShader(const plat::Path& path) = 0;

		virtual void begin() = 0;
		virtual void end() = 0;

		virtual void bind(const Mesh& mesh) = 0;
		virtual void bind(const Shader& shader) = 0;

		virtual void draw(uint32_t count, uint32_t offset = 0) = 0;

		static Ref<Graphics> make(plat::Window* pWindow);
	protected:
		Graphics() noexcept = default;
	};
}    
