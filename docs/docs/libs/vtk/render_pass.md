---
sidebar_position: 6
---

# Render Pass

## Construction

```cpp
vtk::Ref<vtk::RenderPass> renderPass = vtk::RenderPassBuilder(mDevice)
	.beginSubpass()
	.addColorAttachment(swapchain->format())
	.endSubpass()
	.build();
```

:::note
Can construct an indefinite number of subpasses; however, current functionality is limited to basic color attachments.
Will be expanded upon on a later date.
:::