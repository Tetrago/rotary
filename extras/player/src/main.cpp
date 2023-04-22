#include "vtk/pipeline.hpp"
#include <stdexcept>
#include <plat/plat.hpp>
#include <rotary/rotary.hpp>
#include <vtk/vtk.hpp>

int main()
{
	plat::Window window(1024, 576, "Rotary Player");
	rot::Ref<rot::Graphics> graphics = rot::Graphics::make(&window);

	rot::Ref<rot::Shader> shader = graphics->createShader("data/color.vert.spv", "data/color.frag.spv");

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

		graphics->begin();

		graphics->bind(*shader);
		graphics->draw(3);
		
		graphics->end();

		plat::Window::update();
	}

	return 0;
}
