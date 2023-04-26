---
sidebar_position: 2
---

# Instance

## Construction

```cpp
std::span<std::string_view const> extensionNames;
void debug_callback(vtk::Severity severity, const std::string& message);

// ...

vtk::Ref<vtk::Instance> instance = vtk::InstanceBuilder()
    .application("Player", 0, 1, 0) // (Major, Minor, Patch) version
    .engine("Rotary", 0, 1, 0)
    .extensions(extensionNames)
    .debug(debug_callback)
    .build();
```

Invoking the `debug` option enables the `VK_EXT_debug_utils` extension and `VK_KHRONOS_validation_layer` layer,
and directs all output through the debug callback.

:::info
The Vulkan Toolkit utilizes the callback as well to report internal errors and warning messages.
:::

## See Also

- [Platform Toolkit: Vulkan](/libs/plat/vulkan)