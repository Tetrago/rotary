#include <stdexcept>
#include <plat/plat.hpp>
#include <vtk/vtk.hpp>

int main()
{
	plat::Window window(1024, 576, "Rotary Player");

	vtk::Instance instance = vtk::InstanceBuilder()
		.application("player", 0, 1, 0)
		.engine("rotary", 0, 1, 0)
		.extensions(plat::get_required_instance_extensions())
		.build();

	vtk::PhysicalDevice physicalDevice = [&]()
	{
		if(auto opt = vtk::PhysicalDeviceSelector(instance)
		   .requiredDiscrete()
		   .select())
		{
			return opt.value();
		}

		throw std::runtime_error("Could not find a suitable physical device");
	}();

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
