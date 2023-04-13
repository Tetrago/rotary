#include <stdexcept>
#include <plat/plat.hpp>
#include <plat/vulkan.hpp>
#include <vtk/vtk.hpp>

int main()
{
	plat::Window window(1024, 576, "Rotary Player");

	vtk::Ref<vtk::Instance> instance = vtk::InstanceBuilder()
		.application("player", 0, 1, 0)
		.engine("rotary", 0, 1, 0)
		.extensions(plat::get_required_instance_extensions())
		.debug()
		.build();

	VkSurfaceKHR surface = plat::create_window_surface(window, *instance);

	vtk::PhysicalDevice physicalDevice = [&]()
	{
		if(auto opt = vtk::PhysicalDeviceSelector(*instance)
		   .requiredDiscrete()
		   .requireGraphicsSupport()
		   .requirePresentSupport(surface)
		   .select())
		{
			return opt.value();
		}

		throw std::runtime_error("Could not find a suitable physical device");
	}();

	vtk::Ref<vtk::LogicalDevice> logicalDevice = vtk::LogicalDeviceBuilder(instance, physicalDevice)
		.addGraphicsQueue()
		.addPresentQueue(surface)
		.addSwapchainSupport()
		.build();

	bool running = true;
	while(running)
	{
		plat::WindowEvent event;
		while(window.poll(event))
		{
			switch(event.type)
			{
			case plat::WindowEvent::Close:
				running = false;
				break;
			}
		}

		plat::Window::update();
	}

    return 0;
}
