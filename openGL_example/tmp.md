# Texture
it is hard to draw some object in picture just only setting the vertices. because it will need too many vertices.
not only vertex's location, but also color, texture info are needed.

-> Texture mapping 
paste the image to the area of vertices.

## Texture coordinates
- the texture image position respond to the vertex area.
it is normalized to [0, 1]. The left-down point is origin.

the Texture coordinate is entered with vertex location like a form of vertex attributes to the vertex shader.

During Rasterization, each pixel's texture coordinate value is calculated.

In fragement shader, the texture image color is taken according to the texture coordinate.

## Texture wrapping
How to treat the texture coordinate value out of [0, 1]?

- `GL_REPEAT`
- `GL_MIRRORED_REPEAT`
- `GL_CLAMP_TO_EDGE`
- `GL_CLAMP_TO_BORDER`

Texture coordinate doesn't have to [0, 1]. Just setting how to treat the pixel value out of the range.

## Texture filtering
If the image size doesn't fit the screen, we need a setting option about What texture pixel should be used .

- `GL_NEAREST`: The nearest pixel to the texture coordinate.
-> the pixel edge might be seen.
- `GL_LINEAR`: The four pixels value around the texture coordinate will be interpolated bilinearly.
-> mildly pixel color changed.

## Texture in openGL
1. openGL texture object create / binding
2. Setting wrapping, filtering option
3. Copy the image data to the GPU Memory
4. the texture wanted to use when the shader is binded is sended to the program as uniform.

# Example
## image loading
1. stb include.
stb_image.h : Library to load the image format jpg, png, tga, etc.
it is single-file public domain library. so easy to use.

## Image class design

## Texture application
- Add a texture coordinate to vertex attribute
- Making shader that reads the texture and defining the pixel value.

## openGL texture API 
- glGenTextures(): openGL texture object create
- glBindTexture(): Texture we want to use bind to context
- glTexParameteri(): Setting the parameter of texture filter and wrapping method.
- glTexImage2D(target, level, internalFormat, width, height, border, format, type, data)
	: Send the texture data from CPU Memory to GPU memory with how to use the data.
	set the Binded texture's size, pixel format and copy the image data to GPU.
	target -> Binding texture
	level -> setting texture level. 0 is default. it is related to mipmap.
	internalFormat : Texture's pixel format used by GPU
	width / height / border : set Texture width, height. border size
	format : pixel format of the image
	type : channel data type of the input image
	data : memory address written the image data.

- the power of 2 size is the most efficient for GPU.
- NPOT(Non-Power-Of-Two) texture : NPOT cases might be unsupported depending on the GPU spec.

# Texture Refactoring
We will use the image data just one time. so, smartpointer is too heavy to use for only making texture.

in our class, we use just a pointer to the image.

## Checker image creation
- Let's make a image not only downloading.
- Create image of checker board.

## Mipmap
when the checker board is shrinked, unknown lines are maded.
- when the texture pixel area is bigger than pixel in screen, it is ok.
- But the screen pixel include more than one texture pixel, something goes wrong.

-> That's why we use mipmap.
mipmap: preparation of small size image.
when the screen size get smaller, according to its size the smaller image will be used to prevent the image distortion.

original size level is a base level.
Then, Calculate the half of the width, height image and stored it. (level++)
-> 512 * 512 image has lv0 ~ 9 will be created.

## MULTIPLE TEXTURE
Blending a multiple textures in fragment shader.

### How to provide the texture to the shader program
The number of texture that can be used at a frame in one shader is 32.
There is 32 slot to store the textures.

1. glActiveTexture(textureSlot)
- choose the texture slot and activate the slot.
2. glBindTexture(textureType, textureId) 
- Binding the texture object to the texture slot.
3. glGetUniformLocation() 
- get the uniform handle in the shader
4. glUniform1i() 
- input the texture slot index to sampler2D uniform

## The change in context.cpp
1. vertices
we deliver just 6 element per vertex.
But in this case, we need to inform the vertex location, color, and the texture coordinate.
-> 8 element per vertex needed.

2. SetAttrib.
inform the data's format to GPU. 
by `glVertexAttribPointer()`, `glEnableVertexAttribArry`,
because we added the texture coordinate, let GPU know what is texture coordinate.

3. Loading the image.
by using stb function, load the image data to our program.

4. Making texture object
generate, bind, and set the texture information by `glGenTextures`, `glBindTexture`, `glTexParameteri`.
