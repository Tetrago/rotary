---
sidebar_position: 3
---

# Shader Cross-Compiler

```cpp
#include <shcc/shcc.hpp>
```

## Purpose

The Shader Cross-Compilater library is resposible for handling the loading, compiling, translating, and analysis of shaders.
Because the Rotary aims to support multiple graphics APIs, multiple shader formats will be needed as well.

### Pipeline

```mermaid
flowchart TD
    subgraph "Resource"
        hlsl[(HLSL)]
    end

    glsl([GLSL])
    spirv([SPIR-V])
    dxil([DXIL])
    reflect([Reflection])

    spirv_cross{{SPIR-V Cross}}
    glslang{{Glslang}}
    dxc{{DirectX Shader Compiler}}

    subgraph "Graphics API"
        opengl[/OpenGL/]
        vulkan[/Vulkan/]
        directx[/DirectX/]
    end

    hlsl --- glslang & dxc
    glsl --- opengl
    spirv --- vulkan & spirv_cross
    spirv -.- opengl
    dxil --- directx

    spirv_cross --- glsl & reflect
    glslang --- spirv
    dxc --- dxil

    click glslang "https://github.com/KhronosGroup/glslang" _blank
    click spirv_cross "https://github.com/KhronosGroup/SPIRV-Cross" _blank
    click dxc "https://github.com/microsoft/DirectXShaderCompiler" _blank

    style glsl fill:#f96
    style dxil fill:#f96
    style opengl fill:#f96
    style dxc fill:#f96
    style directx fill:#f96
```

# Usage

```cpp
void callback(const std::string& message);
std::string source; // HLSL source

// ...

std::shared_ptr<shcc::Runtime> runtime = shcc::runtime(callback);

shcc::Package package = runtime->package(source, { shcc::Stage::Vertex, shcc::Stage::Fragment });

std::vector<uint8_t> vertexSpvBinary = package.compile(shcc::Stage:Vertex, shcc::Language::Spirv);
std::span<uint32_t> binary(reinterpret_cast<const uint32_t*>(vertexSpvBinary.data()), vertexSpvBinary.size() / 4);

shcc::Resources resoucres = package.resources(); // Reflection information
```

:::info
Although shader `shcc::Package`s do depend on their corresponding `shcc::Runtime` to function, they hold a `std::shared_ptr<shcc::Runtime>`
internally, preventing it from being destructed beforehand.
:::