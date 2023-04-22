#pragma once

namespace rot
{
	class Shader
	{
	public:
		virtual ~Shader() noexcept = default;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
	protected:
		Shader() noexcept = default;
	};
}    
