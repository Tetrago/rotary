#pragma once

#include <functional>
#include <memory>
#include <type_traits>

namespace vtk
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	concept Handle = std::is_pointer<T>::value;

	template<typename T> requires Handle<T>
	class Holder
	{
	public:
		Holder() noexcept = default;

		Holder(T handle, std::function<void(T)> deleter) noexcept
			: mHandle(handle)
			, mDeleter(std::move(deleter))
		{}

		~Holder() noexcept
		{
			if(!mHandle) return;
			mDeleter(mHandle);
		}

		Holder(const Holder&) = delete;
		Holder& operator=(const Holder&) = delete;

		Holder(Holder&& other) noexcept
			: mHandle(other.mHandle)
			, mDeleter(std::move(other.mDeleter))
		{
			other.mHandle = nullptr;
		}

		Holder& operator=(Holder&& other) noexcept
		{
			mHandle = other.mHandle;
			mDeleter = std::move(other.mDeleter);

			other.mHandle = nullptr;

			return *this;
		}

		operator bool() const noexcept { return mHandle; }
		operator T() const noexcept { return mHandle; }
		operator const T*() const noexcept { return &mHandle; }

		bool has_value() const noexcept { return mHandle; }
		T value() const noexcept { return mHandle; }
		const T* ptr() const noexcept { return &mHandle; }

		void reset() noexcept
		{
			mDeleter(mHandle);
			mHandle = nullptr;
		}
	private:
		T mHandle = nullptr;
		std::function<void(T)> mDeleter;
	};
}    
