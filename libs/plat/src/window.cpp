#include "plat/window.hpp"

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifndef PLAT_WINDOW_MAX_EVENTS
#define PLAT_WINDOW_MAX_EVENTS 64
#endif

namespace plat
{
	namespace
	{
		int gWindows = 0;

		void push_window()
		{
			if(gWindows++ == 0
			   && !glfwInit())
			{
				throw std::runtime_error("Failed to initialize GLFW");
			}
		}

		void pop_window()
		{
			if(--gWindows != 0) return;
			glfwTerminate();
		}
	}

	Window::Window(int width, int height, std::string_view title)
	{
		mEvents.reserve(PLAT_WINDOW_MAX_EVENTS);

		push_window();

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		mHandle = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		if(!mHandle)
		{
			throw std::runtime_error("Failed to create window");
		}

		glfwSetWindowUserPointer(mHandle, this);

		glfwSetWindowCloseCallback(mHandle, [](GLFWwindow* window)
		{
			pushEvent(window, WindowEvent{ WindowEvent::Close });
		});
	}

	Window::~Window() noexcept
	{
		glfwDestroyWindow(mHandle);
		pop_window();
	}

	Window::Window(Window&& other) noexcept
		: mHandle(other.mHandle)
	{
		other.mHandle = nullptr;
	}

	bool Window::poll(WindowEvent& event) noexcept
	{
		if(mEvents.empty()) return false;

		event = mEvents.back();
		mEvents.pop_back();

		return true;
	}

	void Window::update() noexcept
	{
		glfwPollEvents();
	}

	void Window::pushEvent(GLFWwindow* window, WindowEvent event) noexcept
	{
		Window* pWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		pWindow->mEvents.push_back(event);

		if(pWindow->mEvents.size() <= PLAT_WINDOW_MAX_EVENTS) return;
		pWindow->mEvents.erase(pWindow->mEvents.begin());
	}
}    
