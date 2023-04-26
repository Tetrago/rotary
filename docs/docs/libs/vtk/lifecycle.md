---
sidebar_position: 1
---

# Lifecycle

## Handles

The vast majoirty of the handles provided by the library will be stored within a `vtk::Ref<T>`.
For all intents and purposes, this is a `std::shared_ptr<T>` that exists purelty to ensure the proper order of destruction.

For example, a `vtk:Swapchain` contains a `vtk::Ref<LogicalDevice>` which in and of itself holds a `vtk::Ref<Instance>`.
Therefore, even if the user discards their `vtk::Instance` handle, the object will persist until the relying objects are destructed.

## Holders

For lower level and common Vulkan objects, such as `VkFence`s or `VkSemaphore`s, it isn't realistic to create dedicated wrapper objects,
nor is it particularly useful. Instead, such objects are contained within a `vtk::Holder<T>`, which binds a destruction function to the handle,
automatically destructing the underlying type appropriately.

:::caution
`vtk::Holder<T>`s **do not** store `vtk::Ref<T>` handles to their dependencies, meaning any created objects should be manually
destructed before the relavent dependencies, with the `vtk::Holder<T>::reset` method, if necessary.
:::