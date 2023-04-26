---
sidebar_position: 2
---

# ShaderX

## Description

ShaderX is a command-line utility intended to expose the featues of the [Shader Cross-Compiler](/libs/shcc) outside of the engine's runtime
in order to ease the process of managing and loading shaders.

## Commands

### Dump

Compiles the given HLSL source files as a vertex and fragment shader and prints the relavent reflection info.\
Currently only extracts the inputs of the vertex stage. They are printed by their name, element size, and type width respectively.

For example, an input struct such as:

```cpp
struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR0;
};
```

Will be printed as:

```
- position (4x3)
- color (4x4)
```