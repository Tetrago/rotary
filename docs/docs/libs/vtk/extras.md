---
sidebar_position: 9
---

# Extras

## Fences and Semaphore

```cpp
vtk::Holder<VkFence> fence = vtk::create_fence(logicalDevice);
vtk::Holder<VkFence> fence = vtk::create_fence(logicalDevice, true); // Create a pre-signaled fence

vtk::Holder<VkSemaphore> semaphore = vtk::create_semaphore();
```

### See Also

- [Holders](./lifecycle#holders)

## Commands

- `void wait(const LogicalDevice&, VkFence)`

### Command Buffers

- `void reset(const LogicalDevice&, VkFence)`
- `void reset(VkCommandBuffer)`
- `void begin(VkCommandBuffer, bool single)`

    Begins recording a command buffer. `single` parameter indicated whether or not to set the
    `VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT` flag.

- `void end(VkCommandBuffer)`