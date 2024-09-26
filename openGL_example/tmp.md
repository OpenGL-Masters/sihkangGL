# Cubemap
- texture map consist of six 2D texture planes.
- It is useful to draw environment.

from the origin, the 3d direction vector samples texture pixel.
Cubemap uses few vertices to draw image.

In one texture object, six texture datas are attached.

## Environment mapping
Environment mapping is a computer graphics technique used to simulate the appearance of reflective or refractive surfaces by mapping the surrounding environment onto the surface of an object. 

It is commonly used for rendering shiny objects like mirrors, water, glass, and metal surfaces.

If you want to be more dynamic, `Dynamic environment map`is uselful method by changing the image in every frames.
Rendering each face of cube map in every frame. (framebuffer will be used)

6 times rendering should be done formerly.

# advanced data managing
part copy: `glBufferSubData(target, offset, size, ptr)`
 
target에 바인딩된 버퍼에 offset부터 size 만큼의 공간에 ptr부터 size 만큼의 데이터를 복사

access directly: `glMapBuffer()`, `glUmmapBuffer()`
glMapBuffer(target, usage)
: target에 바인딩된 버퍼에 usage 목적으로 접근 가능한 데이터 포인터를 가져옴

glUnmapBuffer(target)
: 포인터를 통한 버퍼 접근 종료

copy buffer to buffer`glCopyBufferSubData()`
glCopyBufferSubData(rtarget, wtarget, roffset, woffset, size)
: rtarget에 바인딩된 버퍼의 roffset부터 size 만큼의 데이터를 wtarget에 바인딩된 버퍼의 wtarget부터 size 만큼의 공간에 복사

glTexSubImage2D()
이미 GPU 메모리가 할당된 2D texture에 새로운 CPU 이미지 데이터를 업데이트하고 싶을때 사용하는 함수

# Advanced GLSL
## built-in variable
- gl_PointSize
: size of point
Need `glEnable(GL_PROGRAM_POINT_SIZE)`

- gl_VertexID
: vertex index processing in vertex shader.

- gl_FragCoord
: pixel's location in screen usable in fragment shader.

- gl_FrontFacing
: In fragment shader. 
It is a bool data that whether the current pixel is front or back.

It is useful when use different material back and front.

- gl_FragDepth
: Setting the pixel's depth value menually. in fragment shader.
But if you use gl_FragDepth, early depth test is disable.

- Interface block
: the link in/out variables between vs and fs is important.
Interface block is for managing these variables.
It looks like struct data type.

If you use the interface block, you should make the block name same in vs and fs. (but the struct variable name can be different in vs and fs)

## Uniform buffer object(UBO)
if you use UBO, you can set the global unform value that could be used in every shader.

glGenBuffers() -> Bind `GL_UNIFORM_BUFFER` target. -> copy the data by using glBufferData(), etc.

There is memory layout setting for efficiency of GPU Memory access. User can modify it freely.
e.g.) layout (std140)

## Geometry shader
**Geometry Shader** is the step of shader between **Vertex Shader** and **Fragment Shader** in the openGL pipeline. It provides the function that create or modify the new geometrical structure based on the given primitives.

### Geometry Shader's features

1. **processing primitives**:
   Geometry Shader is given the primitives like **Point**, **Line**, **Triangle**. It can make another primitive or modify by processing given primitives. for example, It can expand from one point to several points. and can make new triangles by receiving the triangles data.

2. **create and modify the primitive**:
   - Geometry Shader can make new vertices by givin geometry data(triangles, lines, etc.)

- **advantages**:
   - Can make the new geometry data directly in shader step.
   - by modifying the primitives, geometry structure can be expanded dynamically

- **limitation**:
   - performance budget. especially, making a lot of vertices causes declining the frame rate.
   - Geometry Shader operate complex calculation. So, the performance is worse than tesselation shader.

## instancing
If you want to draw the same object many times, Calling glfunciton several times make the performance bad.

Instancing draw the several object just one function call.
It reduces the communication between CPU and GPU.

`glDrawArraysInstanced()`, `glDrawElementsInstanced()`
the number of instance will be passed by argument.

- `gl_InstanceID`
Built-in variable in vertex shader.
It divides each instances.

There is two instancing method.

1. drawing by uniform.
This method delivers the instancing data to uniform.
It is easy to implement. But not efficient.

2. drawing by VBO.
the instancing data will be contained in VBO.
It is better performance because GPU manage the data directly.

But it is more complicated and use more memory.

## Anti-Aliasing
make image soft.

### Supersampling Anti-Aliasing (SSAA)
rendering bigger resolution image .
Downsampling the rendered image.

### Multisample Anti-Aliasing (MSAA)
anti-aliasing method that OpenGL provides.

in one pixel, investigate the several sample that the sample position is in the triangle.

the basic framebuffer(screen) supports multi-sample.
But if you want to Off-screen MSAA, make the framebuffer, texture, render buffer as multi-sample version.