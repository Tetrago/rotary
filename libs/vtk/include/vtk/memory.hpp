#pragma once

#include <memory>

namespace vtk
{
	template<typename T>
	using Ref = std::shared_ptr<T>;
}    
