---
sidebar_position: 7
---

# Framebuffer

## Construction

```cpp
VkExtent2D extent;
VkImageView view;

// ...

vtk::Ref<vtk::Framebuffer> framebuffer = vtk::FramebufferBuilder(logicalDevice, renderPass)
    .size(extent)
    .view(view)
    .build();
```

### Swapchain Example

```cpp
vtk::Ref<vtk::Swapchain> swapchain;

// ...

std::vector<vtk::Ref<vtk::Framebuffer>> framebuffers(swapchain->views().size());

for(int i = 0; i < framebuffers.size(); ++i)
{
    framebuffers[i] = vtk::FramebufferBuilder(logicalDevice, renderPass)
        .size(swapchain->extent())
        .view(swapchain->views().at(i))
        .build();
}
```