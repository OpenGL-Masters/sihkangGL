# Learning openGL

---
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

---

<details>
<summary> # [W02] </summary>

## including external library by using cmake

## add GLFW DEPENDENCY 
- openGL is just an API for graphic. 
- it doesn't have a features like making window, creating and connect surface, and connecting keyboard, mouse on windnow.

for these utilities, we need **GLFW** library.
it will be included by cmake. It is easy to connect to our project.

### At cmake file to add external library dependency
<details><summary> Dependency.cmake file </summary>

```
# include on below means we want to add a external project.
# this time, include can help for us to use a ExternalProject_Add feature.
include(ExternalProejct)

# on below, setting the variables about dependency.
set(DEP_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(DEP_INCLUDE_DIR ${DEP_INSTALL_DIR}/include)
set(DEP_LIB_DIR ${DEP_INSTALL_DIR}/lib)

# ExternalProject_Add function make connection between our project and external git project. automatically external project will be made by cmake. and we also setting the cmake option of external project.

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
		-DGLFW_BIULD_EXAMPLES=OFF 
		-DGLFW_BUILD_TESTS=OFF
		-DGLFW_BUILD_DOCS=OFF # 해당 옵션들은 glfw 깃에 들어가서 CMakeLists.txt에 적혀있다. -D 플래그를 통해 해당 옵션들을 수정해주는것.
	)

# setting DEP_@@@@ variables as ${DEP_@@@@} + ###.
# adding dependency list and library file list.
set(DEP_LIST ${DEP_LIST} dep_glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)
```

</details>

if you want to include external libraries, Separate the cmake file CMakeLists.txt and Dependency.cmake. it will be more useful to control the external and the internal(my project).

above code is also typed in *dependency.cmake* file.

if the code separated to two files, you should include the external dependency on your CMakeLists.txt.

```
# On CMakeLists.txt, include dependency.cmake
include(Dependency.cmake)

# Setting the directories having headers, libraries.
# In order, 
# location of header files → set the headers that will be used for code by compiler.
# location of library → 링커가 필요한 라이브러리 파일을 찾을 수 있도록 지정.
# set using library → 실제로 컴파일할 때 링커가 참조할 라이브러리를 명시.
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})
```

On CmakeLists.txt, we can set the information about window.

```

# glfw window setting information
set(WINDOW_NAME "Hello, OpenGL!")
set(WINDOW_WIDTH 960)
set(WINDOW_HEIGHT 540)

# it is same with "#define WINDOW_NAME "HELLO OPENGL"
# it replace to define macro.
target_compile_definitions(${PROJECT_NAME} PUBLIC
	WINDOW_NAME="${WINDOW_NAME}"
	WINDOW_WIDTH=${WINDOW_WIDTH}
	WINDOW_HEIGHT=${WINDOW_HEIGHT}
	)

# Dependency files (${DEP_LIST}) have to be built before the my project.
add_dependencies(${PROJECT_NAME} ${DEP_LIST})

```

as i mentioned, I've used macOS. it need to explicitly link the framework we need.

on dependency.cmake,

```
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

1. Cocoa:
- Framework for construction user interface.

2. IOKit:
- Framework for the work associated with hardware.
- interaction with external devices, Monitoring the hardware status.
- display, keyboard, mouse, etc.

3. CoreFoundation:
- Framework for low level data struction, and utilites.
- provide some tools like basic data type, string, date, collection(array, dictionary, etc.)
- It helps interaction between the other framework of macOS to provide various data types and algorithms.


## adding glad dependency
same above.

but, you need to include glad header former than glfw.
because glfw use the function pointer of glad, you should include glad for the loading of the openGL function pointer.


## Let's booting the window.

1. glfwInit()
it is the function that initialize glfw library.
The first step for using glfw.

2. glfwWindowHint()
this function gives information about window setting.
It makes window by using the hint we give delivered by glfwWindowHint().
It can contain the GLFW_CONTEXT_VERSION, openGL profile, forward_compat, etc.

3. glfwCreateWindow
Let's create the window we want.
the five parameters contain window width, height, name, etc.

4. glfwMakeContextCurrent()
it select the window that we want to work.
the window delivered by arguments. then the window will be selected current context.

5. gladLoadGLLoader()
This function load the openGL function to the process that delivered by argument.

acutally the function pointer that return the process address will be delivered.

after the gladLoadGLLoader(), we can use the openGL functions.

6. glfwWindowShouldClose()
this function return boolean data means the window should close.
we use this function to looping.

7. glfwPollEvents()
this function checked the event happened in window.
keyboard, mouse event can be polled.

8. glfwTerminate()
close the glfw library.

in this flow, we can add the functions for graphics.
- glClearColor(), glClear() : make the background as the color we choose.

- glfwSwapBuffers() : by using buffer swap, we can express the picture more smoothly

- glfwSetFramebufferSizeCallback()
- glfwSetKeyCallback()
: these callback functions will act on loops. the event happened, callback function activate and return something.

![callback function working](./attachedFiles/image.png)

- glViewport(0, 0, width, height)
this function set the window that will be draw. 
0, 0 means the left - down side point.
width, height means the window size.

if the window size was changed, glViewport should call to set the window again.

</details>

---
