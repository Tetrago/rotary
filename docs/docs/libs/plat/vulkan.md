---
sidebar_position: 2
---

# Vulkan

```cpp
#include <plat/vulkan.hpp>
```

## Window Surface

```cpp
kSurfaceKHR plat::create_window_surface(const Window& window, VkInstance instance);
void plat::free_window_surface(VkInstance instance, VkSurfaceKHR surface);
```

## Instance Extensions

```cpp
std::span<std::string_view const> plat::get_required_instance_extensions();
```

:::danger
Requires at least one existing window in order to call.
:::