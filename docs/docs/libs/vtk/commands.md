---
sidebar_position: 8
---

# Commands

## Command Pool

```cpp
uint32_t index = logicalDevice->queueIndex(vtk::QueueType::Graphics);

// ...

vtk::Ref<vtk::CommandPool> commandPool = vtk::CommandPoolBuilder(logicalDevice)
    .index(index)
    .builder();
```

## Command Buffer

```cpp
VkCommandBuffer buffer = commandPool->create();
std::vector<VkCommandBuffer> buffers = commandPool->create(3);
```

:::caution
`VkCommandBuffer`s do not need to be manually destroyed; however, they only exist for as long as the corresponding `VkCommandPool`, i.e. the origin `vtk::CommandPool` object.
`VkCommandBuffer`s are raw Vulkan handles, so they do not store a reference to the `VkCommandPool` on their own.
:::