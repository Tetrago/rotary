#pragma once

#include "rotary/core/base.hpp"

namespace rot
{
	class ROTARY_API Mesh
	{
	public:
		virtual ~Mesh() noexcept = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
	protected:
		Mesh() noexcept = default;
	};
}    
