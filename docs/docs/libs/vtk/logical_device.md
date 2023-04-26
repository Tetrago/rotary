---
sidebar_position: 4
---

# Logical Device

## Construction

```cpp
VkSurfaceKHR surface;

// ...

vtk::Ref<vtk::LogicalDevice> logicalDevice = vtk::LogicalDeviceBuilder(instance, physicalDevice)
    .addGraphicsQueue()
    .addPresentQueue(surface)
    .addSwapchainSupport() // Adds `VK_KHR_swapchain` extension
    .build();
```