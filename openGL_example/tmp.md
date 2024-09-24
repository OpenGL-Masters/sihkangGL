# Depth Test
Let's learn about Depth test for rendering.
if there are a lot of meshes, whether the obj is in front of or not is important to draw picture.
Depth test is for what is in front of.
if the obj is behind something, it should be hidden.

Depth buffer: buffer storing the depth value(z value)
Depth test: comparing the pixel's depth value and the value stored in depth buffer.
if the result is true, draw the pixel or not.

## depth test function

- Using depth test
`glEnable(GL_DEPTH_TEST);`

- Not using depth test
`glDisable(GL_DEPTH_TEST);` 
== `glEnable(GL_DEPTH_TEST); glDepthFunc(GL_ALWAYS);`
-> the object pixel drawed later cover the pixel drawed previously.

- stop updating depth buffer
`glDepthMask(GL_FALSE);`

- setting the initial value of depth buffer
`glClearDepth(1.0f);`

- setting comparing operator
`glDepthFunc(GL_LESS);`
: GL_ALWAYS, GL_NEVER, GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL

## Depth visualization
### disturbing of depth value
By perspective prjection, the depth value is normalized to 0 ~ 1 divided by w.
-> normalized z value becomes 1/z shape.
it makes disturbing of value.(Z-fighting)

### solution of Z-fighting
1. separate each plane not so close.
2. do not set the camera near value too small.
3. use more accurate depth buffer(use more bits) for precise value expression

# Stencil buffer
8-bit integer buffer that make draw on certain pixels.

## Stencil test
do stencil test before depth test.
There is many usage of stencil test.

```c
glEnable(GL_STENCIL_TEST); // init stencil test
glStencilMask(0xFF);		// setting the stencil buffer's bit updated.
glStencilFunc(GL_EQUAL, 1, 0xFF); // set the function of stencil test
```

- `glStencilOp(sfail, dpfail, dppass)`
sfail: operation when the stencil test failed
dpfail: operation when the stencil test passed but depth test failed.
dppass: operation when stencil and depth test passed.

- GL_KEEP: keep the current stencil buffer.
- GL_ZERO: change the stencil buffer to 0.
- GL_REPLACE: replace the value set by glStencilFunc.
- GL_INVERT: Bitwise invert of stencil buffer value.
- GL_INCR: plus 1 at stencil buffer value. (if the value is max, stay)
- GL_INCR_WRAP: plus 1 but if the value is max, go to 0.
- GL_DECR: minus 1 stencil buffer val. min-> stay.
- GL_DECR_WRAP: minus 1 stencil buffer val. min -> 255.

## Usage of Stencil buffer - outlining
- clear the stencil buffer to 0.
- set stencil test GL_ALWAYS.
- the drawn part's stencil value set to 1.
- object rendering
- depth test disable / stencil update disable
- make object slightly bigger and set the shader for outline.
- object rendering for the pixel that the stencil value is not 1.
- depth test enable / stencil function GL_KEEP.

```cpp
	// set the stencil test options
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	// draw box -> the pixel of box passes stencil test
	modelTransform =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 2.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
	transform = projection * view * modelTransform;
	m_program->SetUniform("transform", transform);
	m_program->SetUniform("modelTransform", modelTransform);
	m_box2Material->SetToProgram(m_program.get());
	m_box->Draw(m_program.get());

	// slight bigger box's pixel draw that passes stencil test. -> outline.
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	m_simpleProgram->Use();
	m_simpleProgram->SetUniform("color", glm::vec4(1.0f, 1.0f, 0.5f, 1.0f));
	m_simpleProgram->SetUniform("transform", transform * 
		glm::scale(glm::mat4(1.0f), glm::vec3(1.05f, 1.05f, 1.05f)));
	m_box->Draw(m_simpleProgram.get());

	// return the setting 
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
```

# Blending
- calculate the pixel we want to draw and the pixel in framebuffer.
- for drawing apparent objects.

- init
`glEnable(GL_BLEND)`
- blending function
`glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`
-> can set the F value of blending formula.

* Blending formula 
`C_result = C_source * F_source (+) C_destination * F_destination`

glBlendEquation function can set the middle operator of the formula.

glBlendFuncSeparate can set the each color and alpha value.

## problem by depth test.
the order of drawing can affect apparency.
-> Sort the object drawing order by looking direction(order-independency transparency)

### face culling
generally CCW is front face.
-> do not draw the back face.

```c
glEnable(GL_CULL_FACE); // on face culling
glDisable(GL_CULL_FACE); // off face culling
glFrontFace(GL_CCW); // set front face as CCW
glCullFace(GL_BACK); // do not draw back face
glCullFace(GL_FRONT); // do not draw front face
```

# FrameBuffer
: merge color / depth / stencil buffer to framebuffer.

When the openGL context generated by GLFW, the framebuffer for drawing on window generated automatically.

developer can use and generate framebuffer directly.
-> for post-processing.
-> Using the rendering scene as texture.

## Method of using framebuffer
- generate framebuffer object
- generate render buffer(color / depth / stencil)
- connect render buffer to framebuffer.
- check the render buffer combination can be used for framebuffer.
