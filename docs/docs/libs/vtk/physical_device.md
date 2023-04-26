---
sidebar_position: 3
---

# Physical Device

## Selection

```cpp
VkSurfaceKHR surface;

// ...

std::optional<vtk::PhysicalDevice> physicalDevice = vtk::PhysicalDeviceSelector(instance)
    .requireDiscrete()
    .requireGraphicsSupport() // Checks for a graphics queue
    .requirePresentSupport(surface) // Checks for a queue that supports presenting to the given surface
    .filter([](const vtk::PhysicalDevice& device) -> bool { ... }) // Allows the user to check for any additional conditions
    .select();
```