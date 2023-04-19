#pragma once

#include <memory>

namespace rot
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	inline Ref<T> make_ref(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}    
