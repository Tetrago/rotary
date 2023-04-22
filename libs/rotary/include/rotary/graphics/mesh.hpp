#pragma once

namespace rot
{
	class Mesh
	{
	public:
		virtual ~Mesh() noexcept = default;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
	protected:
		Mesh() noexcept = default;
	};
}    
