#include <stdexcept>
#include <plat/plat.hpp>
#include <rotary/rotary.hpp>

int main()
{
	plat::Window window(1024, 576, "Rotary Player");
	rot::Ref<rot::Graphics> graphics = rot::Graphics::make(&window);

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
		graphics->end();

		plat::Window::update();
	}

	return 0;
}
