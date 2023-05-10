#pragma once

#include "rotary/core/base.hpp"

namespace rot
{
	class ROTARY_API Shader
	{
	public:
		virtual ~Shader() noexcept = default;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
	protected:
		Shader() noexcept = default;
	};
}    
