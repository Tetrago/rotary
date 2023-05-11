#pragma once

#include <vector>
#include <string_view>

struct GLFWwindow;

namespace plat
{
	struct WindowEvent
	{
		enum Type
		{
			Close
		};

		Type type;
	};

	class Window
	{
	public:
		Window(int width, int height, std::string_view title);
		~Window() noexcept;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&& other) noexcept;
		Window& operator=(Window&&) = delete;

		operator GLFWwindow*() const noexcept { return mHandle; }

		bool poll(WindowEvent& event) noexcept;

		static void update() noexcept;
	private:
		static void pushEvent(GLFWwindow* window, WindowEvent event) noexcept;

		GLFWwindow* mHandle = nullptr;
		std::vector<WindowEvent> mEvents;
	};
}

#ifdef PLAT_VULKAN
#include <vulkan/vulkan.h>

namespace plat
{
	std::vector<std::string_view> get_required_instance_extensions() noexcept;
	VkSurfaceKHR create_window_surface(const Window& window, VkInstance instance);
	void free_window_surface(VkInstance instance, VkSurfaceKHR surface) noexcept;
}    
#endif
