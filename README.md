# Learning openGL

<details>
  <summary># [W01] cmake_project_example</summary>

- I learned how to set up OpenGL on an M1 MacBook.
- CMake will be used for configuring and building projects.
- I will create a `CMakeLists.txt` file and configure and build the project using the command palette in VSCode.
- OpenGL will be compiled with Xcode (Apple supports OpenGL up to version 4.1).

## What should I learn this week?
In this chapter, I will set up my OpenGL environment on an M1 Mac running macOS.

I will use VSCode to progress in coding for graphics programming.

## Setup
Some extensions are needed in VSCode.

1. C/C++ extension.
2. ~~C/C++ Intellisense~~: Deprecated. It was merged into the C/C++ extension.
3. CMake.
4. CMake Tools.

## Compilation
On macOS, OpenGL has been officially deprecated. So, I will use OpenGL 3.3 to study computer graphics.
(I heard OpenGL 4.6 was released, but Apple supports only up to 4.1.)

Additionally, I need **Xcode** to compile OpenGL code.

## CMake
It surprised me! At 42 Seoul, I had to manually type all the `Makefile` commands to compile the source code and generate the executable file.
With CMake, it automatically creates the build directory and `Makefile` with just one action.

### CMakeLists.txt
This is a recipe to create the build directory. We can set the following points:

- Define build targets (e.g., `add_executable`, `add_library`).
- Set compiler options and link options.
- Find external libraries (`find_package`).
- Set installation rules.

In this chapter, I created a `CMakeLists.txt` like the one below:

```cmake
# Set minimum version of CMake
cmake_minimum_required(VERSION 3.14)

# Set variable PROJECT_NAME as cmake_project_example and specify C++ version
set(PROJECT_NAME cmake_project_example)
set(CMAKE_CXX_STANDARD 17)

# Define the project. The project() command sets the project name and language version.
project(${PROJECT_NAME})

# Add executable file for ${PROJECT_NAME} using src/main.cpp
add_executable(${PROJECT_NAME} src/main.cpp)
```

### Configuration & Build
In this class, I use the command palette for "CMake: Configure" and "CMake: Build".

1. **Configuration**  
   In this step, the `CMakeLists.txt` is processed, creating the build system (like a `Makefile`), and setting up the environment for the compiler, platform, etc.

2. **Build**  
   The sources are compiled, and object files are linked. All requested targets (libraries, executables, etc.) are built.

In our source code, `main.cpp`, we print "Hello, World!" to stdout. When we execute the built executable, we can see "Hello, World!" printed in the console.


</details>

<details>
<summary> # [W02] </summary>

## Including External Library Using CMake

### Add GLFW Dependency
- OpenGL is just an API for graphics.
- It doesn't have features like creating a window, creating and connecting a surface, or connecting a keyboard and mouse to the window.

For these utilities, we need the **GLFW** library. It will be included using CMake, making it easy to connect to our project.

### In the CMake File to Add External Library Dependency

<details><summary> Dependency.cmake File </summary>

```cmake
# include below adds an external project.
# Here, include allows us to use the ExternalProject_Add feature.
include(ExternalProject)

# Set variables for the dependency.
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# ExternalProject_Add function creates a connection between our project and an external git project. 
# The external project will be automatically handled by CMake, and we also configure the CMake options for the external project.

ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.3"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=OFF 
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF # These options can be found in glfw's CMakeLists.txt. 
        # Using the -D flag, we modify the options.
)

# Setting DEP_@@@@ variables as ${DEP_@@@@} + ###.
# Adding the dependency list and library file list.
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)
```

</details>

If you want to include external libraries, it is better to separate the CMake file into *CMakeLists.txt* and *Dependency.cmake*. This makes managing external and internal libraries easier.

The above code is typed in *dependency.cmake*.

If the code is separated into two files, you need to include the external dependency in your *CMakeLists.txt*.

```cmake
# In CMakeLists.txt, include dependency.cmake
include(Dependency.cmake)

# Set the directories containing headers and libraries.
# In order:
# - location of header files → sets the headers for the compiler.
# - location of library files → specifies the location where the linker can find the necessary library files.
# - sets the libraries → defines the libraries that the linker will reference during compilation.
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})
```

In *CMakeLists.txt*, we can set the information about the window.

```cmake
# GLFW window setting information
set(WINDOW_NAME "Hello, OpenGL!")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

# It's the same as "#define WINDOW_NAME "HELLO OPENGL"
# It replaces with a define macro.
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency files (${DEP_LIST}) must be built before my project.
add_dependencies(${PROJECT_NAME} ${DEP_LIST})
```

As I mentioned, I use macOS. On macOS, you need to explicitly link the required frameworks.

In *dependency.cmake*:

```cmake
if (APPLE)
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)

    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${COCOA_FRAMEWORK}
        ${IOKIT_FRAMEWORK}
        ${COREFOUNDATION_FRAMEWORK}
        ${OPENGL_FRAMEWORK}
    )
endif()
```

1. **Cocoa**:
   - A framework for constructing user interfaces.

2. **IOKit**:
   - A framework for handling hardware-related tasks, such as interacting with external devices and monitoring hardware status (displays, keyboards, mice, etc.).

3. **CoreFoundation**:
   - A framework for low-level data structures and utilities.
   - Provides tools like basic data types, strings, dates, and collections (arrays, dictionaries, etc.).
   - Helps interact between other macOS frameworks by providing various data types and algorithms.


## Adding Glad Dependency

You include it in the same way as GLFW, but you need to include the Glad header before GLFW. This is because GLFW relies on Glad for loading OpenGL function pointers.

## Booting the Window

1. **glfwInit()**
   - Initializes the GLFW library.
   - This is the first step in using GLFW.

2. **glfwWindowHint()**
   - This function provides information about the window settings.
   - It configures the window by using the hints we pass, such as the OpenGL version, profile, forward compatibility, etc.

3. **glfwCreateWindow()**
   - Creates the window we want with parameters like width, height, and window title.

4. **glfwMakeContextCurrent()**
   - Sets the specified window as the current context.

5. **gladLoadGLLoader()**
   - Loads the OpenGL functions for the current context using the given process address loader function.
   - After this call, OpenGL functions can be used.

6. **glfwWindowShouldClose()**
   - Returns a boolean indicating whether the window should close.

7. **glfwPollEvents()**
   - Checks for events such as keyboard or mouse input.

8. **glfwTerminate()**
   - Cleans up and terminates the GLFW library.

In this flow, we can add functions for graphics:
- **glClearColor()** and **glClear()**: Set the background color and clear the window with that color.
- **glfwSwapBuffers()**: Swaps the window's buffers to display the rendered image smoothly.

- **glfwSetFramebufferSizeCallback()** and **glfwSetKeyCallback()**: Set callback functions for window resize or key press events.

![Callback Function Working](./attachedFiles/image.png)

- **glViewport(0, 0, width, height)**
   - Sets the viewport, which determines the area of the window where rendering happens.
   - The first two parameters (0, 0) refer to the lower-left corner of the window, and the other two specify the width and height of the viewport.

   If the window size changes, you must call `glViewport()` again to reset the viewport size.


then, we can get this window.
![Hello openGL window](/attachedFiles/hello_openGL.png)

</details>

<details><summary> # [W03] </summary>

# Graphics Pipeline

- **Application**: The application provides the vertex locations and colors that we want to draw. In 3D graphics, we generally draw pictures using triangles. In this stage, OpenGL functions are called to handle drawing. After this step, the GPU will be used.

- **Geometry**: This stage processes vertex data and defines vertex positions. It determines how the triangle's location is perceived from a certain camera angle by transforming the geometry according to the camera's viewpoint.

- **Rasterization**: Converts transformed vertices into fragments (potential pixels), turning geometric information into pixels.

- **Pixel Processing**: Processes individual pixels and defines their color, often based on lighting, textures, or other effects.

## Programmable Shader

GLSL (OpenGL Shading Language) is used for GPU programming and is available from the OpenGL 3.3 core profile onward. We need to write the shader code directly.

- **Vertex Shader**: Calculates information for each vertex, such as position, normals, and other per-vertex attributes.
- **Fragment Shader**: Calculates information for each pixel, mainly determining its final color.

## OpenGL Shader

We write the code for vertex and fragment shaders and then build the program. Additionally, there is a way to build shaders before loading using SPIR-V (not covered in this class).

## Shader Code Loading

- Read the shader code from a file.
- Create a shader object and assign the shader code to it.
- Compile the shader.
- If compilation fails, report the error.

## In `src/common.h`

This header file contains commonly used functions.

### `std::optional<>`

```cpp
std::optional<std::string> LoadTextFile(const std::string &filename);
```

`std::optional<>` is a feature introduced in C++17. It provides a way to handle variables that might not have a value. You can check whether the variable has a value using the `has_value()` function.

## Shader Class Design

There is an OpenGL shader object. When an instance is created, it takes a filename for loading. If instance creation fails, we need to free the memory. This is managed using C++11 smart pointers.

### Smart Pointers

Smart pointers manage memory automatically (RAII). Explicit deletion is not needed.

- **`std::unique_ptr<>`**: Manages a memory block exclusively. When the object goes out of scope, it is automatically deleted by the destructor. There is only one owner of the memory, and ownership cannot be copied. Use move semantics to transfer ownership.

- **`std::shared_ptr<>`**: Shares ownership of the memory among multiple pointers. The memory is deleted when the last `shared_ptr` owning it is destroyed.

- **`std::weak_ptr<>`**: Provides access to an object managed by a `shared_ptr` without increasing its reference count, preventing cyclic references.

1. The constructor is private to prevent direct instantiation. Only `CreateFromFile()` can create an instance. Use `CreateFromFile` to create a shader object.

2. No `set()` function is provided. Shader data is managed within the shader object and is protected.

```cpp
ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType)
{
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}
```

In the above code, we allocate memory for the shader using a smart pointer (`ShaderUPtr`, which is likely a type alias for `std::unique_ptr<Shader>`). Then, we load the shader file. If loading fails, we return `nullptr`, and the allocated memory is automatically cleaned up when the `unique_ptr` goes out of scope. Otherwise, we return the shader by using `std::move` to transfer ownership.

```cpp
bool Shader::LoadFile(const std::string &filename, GLenum shaderType)
{
    auto result = LoadTextFile(filename);
    if (!result.has_value()) // Checks if the optional has a value.
        return false;

    auto &code = result.value();
    const char *codePtr = code.c_str();
    int32_t codeLength = static_cast<int32_t>(code.length());

    // Create and compile shader
    m_shader = glCreateShader(shaderType);
    glShaderSource(m_shader, 1, &codePtr, &codeLength);
    glCompileShader(m_shader);

    // Check for compile errors
    int success = 0;
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
        SPDLOG_ERROR("Failed to compile shader: \"{}\"", filename);
        SPDLOG_ERROR("Reason: {}", infoLog);
        return false;
    }
    return true;
}
```

`Shader::LoadFile` is used to load the shader file. If loading the shader file using the `LoadTextFile` function fails, it returns `false`.

After `LoadTextFile` is used, the shader source code is provided to OpenGL functions.

We then compile the shader code written in GLSL for use in our program. If the compilation is successful, it returns `true`. Otherwise, it returns an error log and `false`.

In this function, we create a shader object and provide it with the shader source code.

- **`glCreateShader`**: Creates an OpenGL shader object and returns an integer ID for the shader.
- **`glShaderSource`**: Sets the GLSL source code in the OpenGL shader object.
- **`glCompileShader`**: Compiles the shader source code.
- **`glGetShaderiv`**: Retrieves information about the shader, such as its compile status.
- **`glGetShaderInfoLog`**: Retrieves the compilation error log for the shader.
- **`glDeleteShader`**: Deletes the shader object.

## Program Class Design

- We will make a pipeline program connected with a vertex shader and a fragment shader for rendering.
- Enter the two shader files into the program and link them.
- If linking succeeds, create an OpenGL program object; otherwise, free the memory.

In OpenGL, a **program object** is like a container to manage multiple shader files. Shader files should be linked to the program. Then we can operate just one program that controls many shaders. The GPU can execute this program using the `glUseProgram` function.

### OpenGL Program Functions

1. **`glCreateProgram()`**  
   - Creates a new program object and returns the program ID (an integer value).
   - Example: `GLuint programID = glCreateProgram();`

2. **`glAttachShader(GLuint program, GLuint shader)`**  
   - Attaches the shader to a certain program. The program must have shaders attached before linking. In this case, 'attach' means pointing to the shader object's address so that the program can access it. At this moment, it doesn't know whether the shader has been compiled correctly.
   - Example: `glAttachShader(programID, vertexShaderID);`

3. **`glLinkProgram(GLuint program)`**  
   - Links the attached shaders to create the final executable shader program. If the shaders are compiled and linked correctly, this function succeeds.
   - It checks whether the attached shaders have any problems compiling or interacting with other shaders.
   - If this function succeeds, the GPU can perform rendering.
   - Example: `glLinkProgram(programID);`

4. **`glUseProgram(GLuint program)`**  
   - Uses this program for the current OpenGL context. By using this program, rendering will be done according to the shaders linked within.
   - Example: `glUseProgram(programID);`

5. **`glGetProgramiv(GLuint program, GLenum pname, GLint *params)`**  
   - Gets the program information and status, e.g., linking status.
   - Example: `glGetProgramiv(programID, GL_LINK_STATUS, &status);`

6. **`glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)`**  
   - Retrieves the program information log (compilation or linking errors, warnings).
   - Example: `glGetProgramInfoLog(programID, 512, NULL, infoLog);`

7. **`glDetachShader(GLuint program, GLuint shader)`**  
   - Detaches the shader from the program. The shader and program are not deleted—just detached from each other.
   - Example: `glDetachShader(programID, vertexShaderID);`

8. **`glDeleteProgram(GLuint program)`**  
   - Deletes the program object to free the memory.
   - Example: `glDeleteProgram(programID);`

9. **`glValidateProgram(GLuint program)`**  
   - Checks whether the program is valid in the current context. This function is useful for finding errors or debugging.
   - Example: `glValidateProgram(programID);`

10. **`glIsProgram(GLuint program)`**  
    - Checks whether the program is a valid object.
    - Example: `if (glIsProgram(programID)) { /* Program is valid */ }`

Shader instances can be used by another program instance. So, they are designed using `ShaderPtr` formed of `shared_ptr`. A `unique_ptr` has only one owner, but a `shared_ptr` can have multiple owners.

If the pointer variable's data type is `shared_ptr`, a `unique_ptr` can be moved into a `shared_ptr` to share ownership.

## Refactoring by Context Class

**Refactoring** means improving the code to make it easier to understand and to have a clear structure.

Make it a habit to write code that works correctly first, then refactor to improve its structure. If the code gets bigger, it might be hard to reuse the code without proper structuring.

### Context Class Design

- Initialize GLFW / OpenGL Context / GLAD.
- Create objects to draw a picture (shader, program).
- Render.
- Free OpenGL objects to release memory.
- Terminate GLFW / program.

**Goals of the context class**: Manage the OpenGL objects and rendering code.

### VAO, VBO, EBO

- **Vertex Array Object (VAO)**: Like a container for VBO and EBO buffer statuses and attribute configurations.
- **Vertex Buffer Object (VBO)**: A buffer containing vertex information like position, color, etc., which is delivered to the GPU.
- **Element Buffer Object (EBO)**: A buffer delivering information about vertex indices.

1. Create VAO -> Bind it.
2. Create VBO / EBO -> Bind them -> Use `glBufferData` to deliver data to the GPU.
3. Let the GPU know about the delivered buffers. The `glVertexAttribPointer` function gives the GPU information about the buffer, and `glEnableVertexAttribArray` enables rendering with the delivered attributes.

After use, unbind the VAO to allow for rendering with other VAOs.

```cpp
GLuint vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);

// Set up VBO
GLuint vbo;
glGenBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

// Set up EBO
GLuint ebo;
glGenBuffers(1, &ebo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

// Set up vertex attributes
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
glEnableVertexAttribArray(0);

// Unbind VAO (optional, for good practice)
glBindVertexArray(0);
```

![intensive red dot](./attachedFiles/reddot.png)

## Vertex Input

Several kinds of information are needed for vertices: data transferring from CPU to GPU, vertex coordinates, etc.

1. **Prepare the vertex data**: In the lecture, this includes vertices and indices arrays.

2. **Vertex Buffer Object (VBO)**:
   - The `glGenBuffers` function creates a VBO. A VBO is a buffer object that can contain vertex data. Various pieces of information about vertices can be stored in a VBO. This data is stored in memory that the GPU can access (position, normal, tangent, color, texture coordinates, etc.).
   - This function allocates usable GPU memory and returns an integer value referring to the buffer ID. If you want to use this buffer in the current context, bind the buffer to the context using `glBindBuffer`.

3. **Storing the data into the Vertex Buffer Object**:
   - Vertex data in CPU memory should be delivered to the GPU (using the `glBufferData` function).

4. **Vertex Array Object (VAO) - Descriptor Object**:
   - The VAO stores the vertex data structure, like each vertex's byte size, how many bytes between vertices (stride), data type, etc. It holds the information about the buffer.
   - Then, enable the attributes for rendering by using `glVertexAttribPointer` and `glEnableVertexAttribArray`.
   - Images are drawn using the Program, VBO, and VAO.

![yellow_triangle](./attachedFiles/yt.png)

If you want to draw a square, let's draw two triangles. Using a VBO alone will require storing 6 vertices, but this is inefficient because we only need 4 unique vertices.

By using an EBO, we can provide information on how the vertices will be used and reused, reducing redundancy.

The vertex indices in the EBO will be used to draw in order, allowing us to reuse vertices.

![yellow_square](./attachedFiles/ys.png)

The rendering will draw the picture using the context bound with VAO, VBO, and EBO.

The `glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)` function will draw the basic vertex shapes.

- **Primitive Type**: Specifies the type of primitives to render (e.g., `GL_TRIANGLES`).
- **Count**: The number of elements to render, which is the number of indices in the EBO.
- **Type**: Specifies the type of the values in the EBO (e.g., `GL_UNSIGNED_INT`).
- **Indices**: Specifies an offset in a buffer or a pointer to the indices (here, 0 since we're starting from the beginning).

## Buffer Class Design

VBO and EBO share the OpenGL functions (`glGenBuffers`, `glBindBuffer`, `glBufferData`, etc.). Therefore, we can create a class to manage buffers more efficiently.

## `VertexLayout` Class

The VAO manages the vertex buffer information. We can encapsulate this functionality in a `VertexLayout` class to handle VAO configurations and attribute setups.

</details>

<details><summary> # [W04] GLSL </summary>

## GLSL (OpenGL Shading Language)

A shader is a small program that runs on the GPU to perform rendering calculations. Rendering operations are executed in parallel across vertices and pixels.

GLSL is the shader language used in OpenGL for coding shaders.

Other shader languages include:

- **HLSL** (High-Level Shading Language) used in DirectX
- **Metal** used in Apple's Metal API
- **Cg** (C for Graphics) by NVIDIA, used in Unity3D

## Basic GLSL Structure

```glsl
#version version_number // Predefined macro

in type in_variable_name;

out type out_variable_name;

uniform type uniform_name;

void main() {
    // Process input(s) and perform graphics operations...
    // Output processed data to output variable
    out_variable_name = processed_data;
}
```

**Type Qualifiers**:

- **`in`**: Input to the shader program.
- **`out`**: Output from the shader program.
- **`uniform`**: Global variables that remain constant for all processed vertices or fragments during a single rendering call.

Shader code also has the `main()` function as an entry point. The shader's output should be stored in an `out` variable.

## GLSL Data Types

- **Basic Types**: `int`, `float`, `double`, `uint`, `bool`
- **Vector Types**:
  - `vecX` (float vectors), `bvecX` (bool vectors), `ivecX` (int vectors), `uvecX` (uint vectors), `dvecX` (double vectors)
  - `X` can be 2, 3, or 4, indicating the number of components.
- **Matrix Types**:
  - `matX`, `bmatX`, `imatX`, `umatX`, `dmatX`
  - `X` can be 2, 3, or 4, representing the dimensions of the square matrix.

### Vectors

- **Accessing Vector Elements**:
  - Use `.x`, `.y`, `.z`, `.w` to access each component.
  - Swizzling allows you to reorder or replicate components (e.g., `vec4 v; vec3 v_xyz = v.xyz;`).
  - Swizzling can also be done using `.rgba` for colors or `.stpq` for texture coordinates.

**Examples of Swizzling**:

```glsl
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyx;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```

**Initializing Vectors**:

```glsl
vec2 vect = vec2(0.5, 0.7); // vec2 constructor
vec4 result = vec4(vect, 0.0, 0.0); // Using another vector to initialize
vec4 otherResult = vec4(result.xyz, 1.0); // Vector swizzling + another component
```

When using other vectors in vector initialization, the basic data types should be the same.

## `in` / `out`

Shaders use `in` and `out` qualifiers to define inputs and outputs.

Every shader should declare the correct `in`/`out` variables to interface with other shader stages.

### Vertex Shader

- Inputs are the vertex attributes provided by the application.
- You can set the attribute index using the following syntax:

  ```glsl
  layout(location = n) in type variable_name;
  ```

- **Important**: The vertex shader must set the output position `gl_Position`. If not, the vertex shader will not compile.

- **Rasterization**: The output of the vertex shader is used to interpolate primitives and calculate data for each pixel.
  
### Fragment Shader

- The interpolated data from the rasterization stage is provided as inputs to the fragment shader.

- For example, if the vertex shader outputs:

  ```glsl
  out vec4 vertexColor;
  ```

  Then the fragment shader should have:

  ```glsl
  in vec4 vertexColor;
  ```

  This linkage is important for passing data between shader stages.

**Uniforms**: Global variables that can be accessed by all shader stages. They hold constant values during the rendering of a primitive and are shared among all shader invocations.

To set the value of a uniform variable:

1. Get the uniform location using `glGetUniformLocation`.
2. Bind the program using `glUseProgram`.
3. Set the uniform value using `glUniform*` functions.

## Vertex Attributes

There is a lot of vertex information:

- Position
- Normal
- Tangent
- Color
- Texture Coordinates
- etc.

Each of them is a vertex attribute.

At this point, we are creating a color attribute for each vertex to pass color data from the vertex shader to the fragment shader.

![vertex color attrib](./attachedFiles/cq.png)


make the color attribution changed every render loops.

![alt text](./attachedFiles/화면-기록-2024-09-14-오후-1.09.39.gif)

---

</details>
