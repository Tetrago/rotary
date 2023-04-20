"use strict";(self.webpackChunkdocs=self.webpackChunkdocs||[]).push([[516],{3905:(e,t,n)=>{n.d(t,{Zo:()=>s,kt:()=>b});var r=n(7294);function o(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function l(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function i(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?l(Object(n),!0).forEach((function(t){o(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):l(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function a(e,t){if(null==e)return{};var n,r,o=function(e,t){if(null==e)return{};var n,r,o={},l=Object.keys(e);for(r=0;r<l.length;r++)n=l[r],t.indexOf(n)>=0||(o[n]=e[n]);return o}(e,t);if(Object.getOwnPropertySymbols){var l=Object.getOwnPropertySymbols(e);for(r=0;r<l.length;r++)n=l[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(o[n]=e[n])}return o}var c=r.createContext({}),u=function(e){var t=r.useContext(c),n=t;return e&&(n="function"==typeof e?e(t):i(i({},t),e)),n},s=function(e){var t=u(e.components);return r.createElement(c.Provider,{value:t},e.children)},p="mdxType",d={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},f=r.forwardRef((function(e,t){var n=e.components,o=e.mdxType,l=e.originalType,c=e.parentName,s=a(e,["components","mdxType","originalType","parentName"]),p=u(n),f=o,b=p["".concat(c,".").concat(f)]||p[f]||d[f]||l;return n?r.createElement(b,i(i({ref:t},s),{},{components:n})):r.createElement(b,i({ref:t},s))}));function b(e,t){var n=arguments,o=t&&t.mdxType;if("string"==typeof e||o){var l=n.length,i=new Array(l);i[0]=f;var a={};for(var c in t)hasOwnProperty.call(t,c)&&(a[c]=t[c]);a.originalType=e,a[p]="string"==typeof e?e:o,i[1]=a;for(var u=2;u<l;u++)i[u]=n[u];return r.createElement.apply(null,i)}return r.createElement.apply(null,n)}f.displayName="MDXCreateElement"},5336:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>c,contentTitle:()=>i,default:()=>d,frontMatter:()=>l,metadata:()=>a,toc:()=>u});var r=n(7462),o=(n(7294),n(3905));const l={sidebar_position:2},i="Project Structure",a={unversionedId:"project-structure",id:"project-structure",title:"Project Structure",description:"",source:"@site/docs/project-structure.md",sourceDirName:".",slug:"/project-structure",permalink:"/rotary/docs/project-structure",draft:!1,editUrl:"https://github.com/tetrago/rotary/tree/develop/docs/docs/project-structure.md",tags:[],version:"current",sidebarPosition:2,frontMatter:{sidebar_position:2},sidebar:"tutorialSidebar",previous:{title:"Intro",permalink:"/rotary/docs/intro"},next:{title:"Platform Toolkit",permalink:"/rotary/docs/libs/plat"}},c={},u=[],s={toc:u},p="wrapper";function d(e){let{components:t,...n}=e;return(0,o.kt)(p,(0,r.Z)({},s,n,{components:t,mdxType:"MDXLayout"}),(0,o.kt)("h1",{id:"project-structure"},"Project Structure"),(0,o.kt)("mermaid",{value:'flowchart TD\n    rotary[(rotary)]\n\n    extras[Extras]\n    player([Player])\n    editor([Editor])\n\n    libs[Libraries]\n    engine([Engine])\n    p2d([Physics2D])\n    p3d([Physics3D])\n    plat([Platform Toolkit])\n    uitk([UI Toolkit])\n    vtk([Vulkan Toolkit])\n\n    subgraph "Third Party"\n        bullet{{Bullet}}\n        imgui{{ImGui}}\n        glfw{{GLFW}}\n        spdlog{{spdlog}}\n        vulkan_headers{{Vulkan-Headers}}\n        vulkan_loader{{Vulkan-Loader}}\n    end\n\n    rotary --- extras & libs\n    extras --- player & editor\n    libs -- rotary --- engine\n    libs -- p2d --- p2d\n    libs -- p3d --- p3d\n    libs -- utk --- uitk\n    libs -- plat --- plat\n    libs -- vtk --- vtk\n\n    player -.-> engine\n    editor -.-> engine & uitk\n\n    engine -.-> p2d & p3d & plat & spdlog & vtk\n    p2d -.-> bullet\n    p3d -.-> bullet\n    plat -.-> glfw\n    uitk -.-> imgui\n    vtk -.-> vulkan_headers & vulkan_loader\n\n    click bullet "https://github.com/bulletphysics/bullet3" _blank\n    click imgui "https://github.com/ocornut/imgui" _blank\n    click glfw "https://www.glfw.org/" _blank\n    click spdlog "https://github.com/gabime/spdlog" _blank\n    click vulkan_headers "https://github.com/KhronosGroup/Vulkan-Headers" _blank\n    click vulkan_loader "https://github.com/KhronosGroup/Vulkan-Loader" _blank\n\n    style editor fill:#f96\n\n    style p2d fill:#f96\n    style p3d fill:#f96\n    style uitk fill:#f96\n\n    style bullet fill:#f96\n    style imgui fill:#f96\n\n    planned[[Planned]]\n    dependency[[Dependency]]\n    submodule[[Submodule]]\n\n    subgraph "Key"\n        planned -- directory --- submodule\n        planned -.-> dependency\n    end\n\n    style planned fill:#f96'}))}d.isMDXComponent=!0}}]);