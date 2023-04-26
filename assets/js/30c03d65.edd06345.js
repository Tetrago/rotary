"use strict";(self.webpackChunkdocs=self.webpackChunkdocs||[]).push([[6],{3905:(e,r,t)=>{t.d(r,{Zo:()=>l,kt:()=>b});var n=t(7294);function a(e,r,t){return r in e?Object.defineProperty(e,r,{value:t,enumerable:!0,configurable:!0,writable:!0}):e[r]=t,e}function i(e,r){var t=Object.keys(e);if(Object.getOwnPropertySymbols){var n=Object.getOwnPropertySymbols(e);r&&(n=n.filter((function(r){return Object.getOwnPropertyDescriptor(e,r).enumerable}))),t.push.apply(t,n)}return t}function o(e){for(var r=1;r<arguments.length;r++){var t=null!=arguments[r]?arguments[r]:{};r%2?i(Object(t),!0).forEach((function(r){a(e,r,t[r])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(t)):i(Object(t)).forEach((function(r){Object.defineProperty(e,r,Object.getOwnPropertyDescriptor(t,r))}))}return e}function c(e,r){if(null==e)return{};var t,n,a=function(e,r){if(null==e)return{};var t,n,a={},i=Object.keys(e);for(n=0;n<i.length;n++)t=i[n],r.indexOf(t)>=0||(a[t]=e[t]);return a}(e,r);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(n=0;n<i.length;n++)t=i[n],r.indexOf(t)>=0||Object.prototype.propertyIsEnumerable.call(e,t)&&(a[t]=e[t])}return a}var s=n.createContext({}),f=function(e){var r=n.useContext(s),t=r;return e&&(t="function"==typeof e?e(r):o(o({},r),e)),t},l=function(e){var r=f(e.components);return n.createElement(s.Provider,{value:r},e.children)},u="mdxType",p={inlineCode:"code",wrapper:function(e){var r=e.children;return n.createElement(n.Fragment,{},r)}},m=n.forwardRef((function(e,r){var t=e.components,a=e.mdxType,i=e.originalType,s=e.parentName,l=c(e,["components","mdxType","originalType","parentName"]),u=f(t),m=a,b=u["".concat(s,".").concat(m)]||u[m]||p[m]||i;return t?n.createElement(b,o(o({ref:r},l),{},{components:t})):n.createElement(b,o({ref:r},l))}));function b(e,r){var t=arguments,a=r&&r.mdxType;if("string"==typeof e||a){var i=t.length,o=new Array(i);o[0]=m;var c={};for(var s in r)hasOwnProperty.call(r,s)&&(c[s]=r[s]);c.originalType=e,c[u]="string"==typeof e?e:a,o[1]=c;for(var f=2;f<i;f++)o[f]=t[f];return n.createElement.apply(null,o)}return n.createElement.apply(null,t)}m.displayName="MDXCreateElement"},2474:(e,r,t)=>{t.r(r),t.d(r,{assets:()=>s,contentTitle:()=>o,default:()=>p,frontMatter:()=>i,metadata:()=>c,toc:()=>f});var n=t(7462),a=(t(7294),t(3905));const i={sidebar_position:7},o="Framebuffer",c={unversionedId:"libs/vtk/framebuffer",id:"libs/vtk/framebuffer",title:"Framebuffer",description:"Construction",source:"@site/docs/libs/vtk/framebuffer.md",sourceDirName:"libs/vtk",slug:"/libs/vtk/framebuffer",permalink:"/rotary/libs/vtk/framebuffer",draft:!1,editUrl:"https://github.com/tetrago/rotary/tree/develop/docs/docs/libs/vtk/framebuffer.md",tags:[],version:"current",sidebarPosition:7,frontMatter:{sidebar_position:7},sidebar:"tutorialSidebar",previous:{title:"Render Pass",permalink:"/rotary/libs/vtk/render_pass"},next:{title:"Commands",permalink:"/rotary/libs/vtk/commands"}},s={},f=[{value:"Construction",id:"construction",level:2},{value:"Swapchain Example",id:"swapchain-example",level:3}],l={toc:f},u="wrapper";function p(e){let{components:r,...t}=e;return(0,a.kt)(u,(0,n.Z)({},l,t,{components:r,mdxType:"MDXLayout"}),(0,a.kt)("h1",{id:"framebuffer"},"Framebuffer"),(0,a.kt)("h2",{id:"construction"},"Construction"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},"VkExtent2D extent;\nVkImageView view;\n\n// ...\n\nvtk::Ref<vtk::Framebuffer> framebuffer = vtk::FramebufferBuilder(logicalDevice, renderPass)\n    .size(extent)\n    .view(view)\n    .build();\n")),(0,a.kt)("h3",{id:"swapchain-example"},"Swapchain Example"),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-cpp"},"vtk::Ref<vtk::Swapchain> swapchain;\n\n// ...\n\nstd::vector<vtk::Ref<vtk::Framebuffer>> framebuffers(swapchain->views().size());\n\nfor(int i = 0; i < framebuffers.size(); ++i)\n{\n    framebuffers[i] = vtk::FramebufferBuilder(logicalDevice, renderPass)\n        .size(swapchain->extent())\n        .view(swapchain->views().at(i))\n        .build();\n}\n")))}p.isMDXComponent=!0}}]);