#include <plat/window.hpp>
#include <vtk/vtk.hpp>

int main()
{
	plat::Window window(1024, 576, "Rotary Player");

	vtk::Instance instance = vtk::InstanceBuilder()
		.application("player", 0, 1, 0)
		.engine("rotary", 0, 1, 0)
		.build();

	vtk::PhysicalDevice physicalDevice = vtk::PhysicalDeviceSelector(instance)
		.requiredDiscrete()
		.select()
		.value();

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
