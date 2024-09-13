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

<details><summary> [W03] </summary>

# Graphics Pipeline
- **Application**: The application provides the vertex locations and colors that we want to draw. In 3D graphics, we generally draw pictures using triangles. In this stage, OpenGL functions are called to handle drawing. After this step, the GPU will be used.

- **Geometry**: This stage processes vertex data and defines vertex locations. It determines how the triangle's location is perceived from a certain camera angle.

- **Rasterization**: Converts vertices into pixels.

- **Pixel**: Processes individual pixels and defines their color.

## Programmable Shader
GLSL (OpenGL Shading Language) is used for GPU programming and is available from OpenGL 3.3 core profile. We need to write the shader code directly.

- **Vertex Shader**: Calculates information for each vertex.
- **Fragment Shader**: Calculates information for each pixel.

## OpenGL Shader
We write the code for vertex and fragment shaders and then build the program. Additionally, there is a way to build shaders before loading using SPIR-V (not covered in this class).

## Shader Code Loading
- Read the shader code from a file.
- Create a shader object and set the shader code.
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

### Smart Pointer?
Smart pointers manage memory automatically (RAII). Explicit deletion is not needed.

- **`std::unique_ptr<>`**: Manages a memory block exclusively. When the memory goes out of scope, it is automatically deleted by the destructor. There is only one owner of the memory, and ownership cannot be copied. Use move semantics to transfer ownership.

- **`std::shared_ptr<>`**: Shares ownership of the memory.

- **`std::weak_ptr<>`**: Allows access to memory without owning it.

1. Constructor is private to prevent direct instantiation. Only `CreateFromFile()` can create an instance. Use `CreateFromFile` to create a shader object.

2. No `set()` function. Shader data is managed within the shader object and is protected.

```cpp
ShaderUPtr Shader::CreateFromFile(const std::string& filename, GLenum shaderType)
{
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}
```

In the above code, we allocate memory for the shader using a smart pointer. Then, we load the shader file. If loading fails, we return `nullptr` to delete the allocated memory. Otherwise, we return the shader memory by using the move function to transfer ownership.

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

After `LoadTextFile` is used, the shader source code information is provided to OpenGL functions.

We then compile the shader code written in GLSL for use in our program. If the compilation is successful, it returns `true`. Otherwise, it returns an error log and `false`.

In this function, we create a shader object and provide it with the shader source code.

- **`glCreateShader`**: Creates an OpenGL shader object and returns an integer ID for the shader.
- **`glShaderSource`**: Sets the GLSL source code to the OpenGL shader object.
- **`glCompileShader`**: Compiles the shader source code.
- **`glGetShaderiv`**: Retrieves information about the shader, such as its compile status.
- **`glGetShaderInfoLog`**: Retrieves the compilation error log for the shader.
- **`glDeleteShader`**: Deletes the shader object.

</details>

<details><summary> # [W03] </summary>

## Program Class Design
- vertex shader, fragment shader 를 연결한 pipeline program을 만들게 될 것.
- 이 프로그램을 이용해서 최종적으로 그림을 그린다
- 두 개의 쉐이더를 입력받아서 프로그램을 링크시킨다.
- 링크에 성공하면 오픈지엘 프로그램 오브젝트를 생성
- 실패하면 메모리 할당 해제

openGL에서 **프로그램(program)**은 여러 쉐이더들의 관리를 위해 존재하는 컨테이너와 같은 존재. 프로그램에 각 쉐이더들을 링크해서 하나로 동작할 수 있게끔 하며, glUseProgram 함수를 통해 GPU에서 해당 프로그램을 실행할 수 있게 함.

### openGL program functions
1. **`glCreateProgram()`**  
   - 새로운 프로그램 객체를 생성하고, 프로그램의 ID를 반환합니다.
   - 예시: `GLuint programID = glCreateProgram();`

2. **`glAttachShader(GLuint program, GLuint shader)`**  
   - 특정 프로그램에 쉐이더 객체를 첨부합니다. 프로그램이 쉐이더를 가지고 있어야 이를 컴파일 후 링크할 수 있습니다. 여기서 첨부란, openGL의 프로그램이 쉐이더에 접근 가능하도록 참조시켜주는 과정을 의미하며 쉐이더 파일을 준비해주는 과정에 해당함. 여기서는 정상 동작 여부를 알지 못함.
   - 예시: `glAttachShader(programID, vertexShaderID);`

3. **`glLinkProgram(GLuint program)`**  
   - 프로그램에 첨부된 쉐이더들을 링크하여 최종 프로그램을 만듭니다. 쉐이더가 제대로 컴파일되고, 프로그램 안에서 유효하게 연결된 상태여야 링크가 성공합니다. 앞서 첨부한 쉐이더가 정상적으로 컴파일되고, 쉐이더 간에 정상적으로 연결되어서 상호작용되는지 확인. 정상적으로 링크가 되는 것은 GPU에서 실행 가능한 상태.
   - 예시: `glLinkProgram(programID);`

4. **`glUseProgram(GLuint program)`**  
   - 지정된 프로그램을 현재 OpenGL 컨텍스트에서 사용합니다. 이 프로그램을 사용하여 이후의 렌더링 작업이 이루어집니다.
   - 예시: `glUseProgram(programID);`

5. **`glGetProgramiv(GLuint program, GLenum pname, GLint *params)`**  
   - 프로그램의 특정 상태나 정보를 가져옵니다. 예를 들어, 링크 상태를 확인할 수 있습니다.
   - 예시: `glGetProgramiv(programID, GL_LINK_STATUS, &status);`

6. **`glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)`**  
   - 프로그램의 정보 로그(컴파일 및 링크 과정 중 발생한 에러 또는 경고)를 가져옵니다.
   - 예시: `glGetProgramInfoLog(programID, 512, NULL, infoLog);`

7. **`glDetachShader(GLuint program, GLuint shader)`**  
   - 프로그램에서 쉐이더를 분리합니다. 링크된 후에는 쉐이더를 분리해도 프로그램은 여전히 유효하게 남아있습니다.
   - 예시: `glDetachShader(programID, vertexShaderID);`

8. **`glDeleteProgram(GLuint program)`**  
   - 프로그램 객체를 삭제합니다. 더 이상 사용하지 않을 때 이 함수를 호출하여 메모리를 정리할 수 있습니다.
   - 예시: `glDeleteProgram(programID);`

9. **`glValidateProgram(GLuint program)`**  
   - 프로그램이 현재 컨텍스트 상태에서 유효한지 확인합니다. 이 함수는 프로그램을 디버그하거나 오류를 찾을 때 유용합니다.
   - 예시: `glValidateProgram(programID);`

10. **`glIsProgram(GLuint program)`**  
    - 프로그램이 유효한 프로그램 객체인지 확인하는 함수입니다.
    - 예시: `if (glIsProgram(programID)) { /* Program is valid */ }`

이 함수들을 통해 프로그램을 생성, 컴파일, 링크, 사용, 삭제 등의 작업을 수행하며, GPU에서 쉐이더 프로그램을 관리하고 실행할 수 있습니다.

앞서 했던 쉐이더 클래스와 거의 동일하게, 프로그램 클래스를 설계해보자.
여러 개의 쉐이더를 링크할 수 있어야 하기 떄문에, 벡터 타입으로 받는다.
벡터 타입은 힙 메모리에 데이터가 저장되어있고, 여러개가 될 수 있다는 것.
-> & 를 붙여서 레퍼런스로 복사되는 오버헤드를 줄이자.

쉐이더 인스턴스는 다른 프로그램 인스턴스에 재사용될 수 있음.
-> ShaderPtr 이라는 쉐어드포인터 형태로 만들었음.

유니크 포인터는 소유권이 단독으로만 존재할 수 있었지만, 쉐어드 포인터는 여럿이 공동으로 소유할 수 있음.

```
#include <memory>

void test()
{
	std::shared_ptr<int> a = std::shared_ptr<int>(new int);
	{
		std::shared_ptr<int> b = a ;
		*b += 1;
		// scope 나가면서 그 메모리를 소유하고 있는 녀석이 몇개인지 확인함.
	}
	// 아래 스코프를 나가게 되면 소유하고 있는 녀석이 0개가 되어 free하게됨.
}
```

쉐이더 클래스같은 경우엔, Shader::CreateFromFile에서 유니크 포인터를 리턴.
이걸 쉐어드로 만들어야하는데, 유니크 -> 쉐어드는 쉽다.
ShaderPtr shader = Shader::CreateFromFile();

받는 녀석이 쉐어드 포인터면 자동적으로 형변환이 일어난다.

a 도 b 도 동일한 메모리 주소값을 가지게 된다.

## Refactoring by context class

기능을 그대로 둔 채로 코드를 좀 더 보기좋게 구조화하는 과정.
동작 하는 코드를 먼저 만들고 보기좋게 고치고. 반복.

코드가 더 큰 규모로 불어나기 전에, 재정비하지 않으면 그 코드를 재사용하는데 어려움이 있을 수 이씅ㅁ.

되는 기능을 먼저 작성하고 잘 돌아감을 확인하면, 보기좋게 정리하는 습관을 들이자.

컨텍스트 클래스를 만들어서 리팩토링 해보도록 하자.
- GLFW / OpenGL Context / GLAD 초기화
- 그림을 그리기 위한 오픈지엘 오브젝트 생성(쉐이더, 프로그램)
- 렌더링
- 오픈지엘 오브젝트 제거
- GLFW 종료 / 프로그램 종료
openGL 오브젝트들을 관리하고 렌더링하는 코드를 분리할 것.

### VAO VBO EBO
Vertex Array Object: VBO, EBO 등 버퍼들에 대한 상태 및 속성을 저장하는 컨테이너
Vertex Buffer Object: GPU로 전달해줄 버텍스 정보를 담는 버퍼.
Element Buffer Object: 버텍스들 간의 정점 인덱스 정보를 전달해주는 버퍼.

우선적으로 VAO를 생성한 뒤, 생성된 VAO를 현재 컨텍스트에서 사용하도록 바인드해준다.
이후 VBO, EBO 를 생성하여 현재 컨텍스트에 바인드해준뒤, CPU 메모리에 올라와있는 버퍼 데이터들을 GPU로 보내는 작업을 진행한다.

보내진 버퍼들에 대해서 GPU가 해석할 방법이 필요하다. 이를 glVertexAttribPointer라는 함수를 이용해 버퍼에 대한 정보를 전달해준뒤, 해당 버텍스의 어떤 속성을 렌더링할지 enable 시켜주는 glEnableVertexAttribArray 함수를 활성화.

이후 사용이 완료된 VAO는 바인드 해제를 하여, 다른 VAO 렌더링에 영향을 주지 않도록 해준다.(현재 강의에서는 렌더링을 하나의 VAO로만 진행하여 해당 부분이 없음.)

```c++
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

## 정점 입력
정점의 위치를 입력하는 방식이 CPU의 데이터를 GPU로 옮기는 것도 필요하고, 데이터를 복사할 때 이 정점이 어떤 좌표계에 있는 점이냐 등 여러가지 정보를 필요로함.

오픈지엘에서는 이를 아래와 같이 처리함.

1. 정점 데이터 준비 -> 강의에서는 vertices, indices

2. Vertex buffer obj(VBO) 준비
glGenBuffer함수를 통해 VBO를 생성한다. VBO는 정점 데이터를 담은 버퍼 오브젝트로, 정점에 대한 다양한 데이터를 GPU가 접근 가능한 메모리에 저장해둔다. (position, normal, tangent, color, texture, coordinate 등의 정보가 VBO에 담길 수 있음.) GPU에 사용가능한 메모리 공간을 할당하고 그 공간을 가리키는 정수값을 리턴받음. 해당 버퍼를 현재 컨텍스트에서 사용하기 위해선 glBindBuffer 함수를 활용하여 바인드시켜주자.

3. Vertex buffer obj 에 정점 데이터 입력
- CPU memory 상에 있는 정점 데이터를 GPU로 옮기는 작업. (glBufferData)
GPU에 메모리가 따로 존재한다. (비디오 메모리)
거기에 정점 데이터를 복사해주는것

4. Vertex Array obj(VAO) 준비
우리의 정점 데이터의 구조를 알려주는 descriptor obj.
각 정점은 몇 바이트로 구성되고, 정점 간에는 몇 바이트가 떨어져있는지, 정점의 0번 데이터는 어떤 사이즈의 데이터가 몇개 있는 형태인지. 데이터의 자료형, 차원 수 등 어떤속성으로 구성되어있는지 버퍼에 대한 정보를 담게된다. 그 중에서도 어떤 속성을 활성화하여 렌더링을 진행할지 세팅해줘야함. (by glVertexAttribPointer, glEnableVertexAttribArray)
-> Program, VBO, VAO를 사용하여 그림을 그리게 될 것.
삼각형을 그려보았따.
![yello_triangle](./attachedFiles/yt.png)

사각형 -> 삼각형 두개 합쳐서 그리면 되니 정점을 6개를 넘겨서 그려보자.
하지만 이는 사각형을 그리기 위해선 4개만으로도 충분한데 두개나 더 써야하니 비효율적.
직점 정점을 어떻게 사용하게할지 세팅하는, 정점을 재활용하기 위해 사용되는 EBO(index bufffer) 를 사용해보자.

정점을 코너 포인트 4개만 선언을 하고 인덱스 배열을 추가해주자.
인덱스 배열에 있는 것을 삼각형으로 묶어서 처리하게 될 것!

마찬가지로 인덱스 버퍼로 사용할 멤버로 context 클래스에 추가해주기.
이렇게 인덱스 버퍼를 통해 그림을 그리면 `glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)` 함수를 사용하게 된다.

![ys](./attachedFiles/ys.png)

현재 바인딩된 VAO, VBO, EBO를 바탕으로 그림
- 그려낼 기본 primitive 타입
- 그리고자 하는 EBO 내 index 갯수
- index 데이터 형
- 그리고자 하는 EBO의 첫 데이터로부터의 오프셋

버퍼 오브젝트를 좀더 효율적으로 사용하기 위해 리펙토링을 해보자.

## buffer class design
현재 우리가 사용하고 있는 버퍼는 VBO, EBO로 두 가지가 존재한다.
이들은 동일하게 glGenBuffer, glBindBuffer, glBufferData 함수를 이용하기 때문에, 하나의 버퍼 클래스로 만들어서 사용하면 더 깔끔해질 것이다.


## VertexLayout 클래스
VAO는 버텍스 버퍼가 어떤 규격으로 이루어졌는지에 대한 정보를 들고있는데,
버텍스 레이아웃이라는 버텍스 버퍼를 가지고 정보를 가지고 있는 클래스로 만들자.

쉐이더 오브젝트 생성 / 소스 컴파일
프로그램 오브젝트 생성 / 쉐이더 링크
VAO : VBO의 구조에 대한 설명, 바인딩된 VBO, EBO 기억
VBO : 정점 데이터를 GPU 메모리 상에 위치시킨 obj
EBO : 인덱스데이터를 GPU 메모리 상에 위치시킨 obj

</details>