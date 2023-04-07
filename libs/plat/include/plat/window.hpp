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

		bool poll(WindowEvent& event) noexcept;

		static void update() noexcept;
	private:
		static void pushEvent(GLFWwindow* window, WindowEvent event) noexcept;

		GLFWwindow* mHandle = nullptr;
		std::vector<WindowEvent> mEvents;
	};
}    
