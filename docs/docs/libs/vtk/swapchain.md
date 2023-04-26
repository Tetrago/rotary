---
sidebar_position: 5
---

# Swapchain

## Construction

```cpp
VkSurfaceKHR surface;

// ...

vtk::Ref<vtk::Swapchain> swapchain = vtk::SwapchainBuilder(logicalDevice, surface)
    .prefer(VK_PRESENT_MODE_FIFO_KHR) // Defaults to VK_PRESENT_MODE_MAILBOX_KHR, or any other available mode
    .prefer({ width, height }) // Defaults to surface extent
    .build();
```