---
sidebar_position: 1
---

# Window

## Example

```cpp
plat::Window window(1024, 576, "Rotary");

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
```

:::note
The Platform Toolkit **does** currently support the instantiation of multiple windows.
:::

## Events

- Close