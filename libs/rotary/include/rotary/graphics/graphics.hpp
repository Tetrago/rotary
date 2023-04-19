#pragma once

#include <plat/plat.hpp>

#include "rotary/core/memory.hpp"

namespace rot
{
	class Graphics
	{
	public:
		virtual ~Graphics() noexcept = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		virtual void begin() = 0;
		virtual void end() = 0;

		static Ref<Graphics> make(plat::Window* pWindow);
	protected:
		Graphics() noexcept = default;
	};
}    
