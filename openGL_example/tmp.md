# Camera/View space
- 3D 공간을 어느 시점에서 어떤 방향으로 바라볼 것인가를 결정
- 카메라를 조작하기 위한 파라미터로부터 view transform을 유도

## camera parameters
position: 카메라 위치
target : 카메라가 바라보는 중심 위치
up vector : 카메라 화면의 세로 축 방향

그 결과 카메라의 행렬은
camera의 local to world transform의 역행렬.

context::Render() 에서 카메라 view 값을 설정해보자.

```c++
	float x = sinf((float)glfwGetTime() * glm::pi<float>() * 2.0f) * 3.0f;
	auto cameraPos = glm::vec3(x, 0.0f, 3.0f);
	auto cameraTarget = glm::vec3(0.0f, 0.0f , 0.0f);
	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// auto cameraZ = glm::normalize(cameraPos - cameraTarget);
	// auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
	// auto cameraY = glm::cross(cameraZ, cameraX);

	// auto cameraMat = glm::mat4(
	// 	glm::vec4(cameraX, 0.0f),
	// 	glm::vec4(cameraY, 0.0f),
	// 	glm::vec4(cameraZ, 0.0f),
	// 	glm::vec4(cameraPos, 1.0f)
	// );

	// auto view = glm::inverse(cameraMat);
	auto view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
```

위 코드에서 glm이 제공하는 카메라 함수를 이용하자.
우리는 Pos, Target, Up만 제공하고, `glm::lookAt` 함수로 view 메트릭스를 계산.

카메라 위치를 매 렌더마다 새롭게 계산해서 시점을 다르게 볼 수 있음.

# Interactive Camera
키보드, 마우스에 작용하는 카메라를 세팅해보자.
context class에 카메라에 대한 정보를 입력해주자.

```c++
glm::vec3 m_cameraPos { glm::vec3(0.0f, 0.0f, 3.0f) };
glm::vec3 m_cameraFront { glm::vec3(0.0f, 0.0f, -1.0f) }; // 카메라가 어떤 방향으로 바라보고 있다를 나타내기 위함.
glm::vec3 m_cameraUp { glm::vec3(0.0f, 1.0f, 0.0f) };
```

해당 카메라의 값을 입력해주기 위한 public 영역에 `ProcessInput(GLFWwindow* window)` 함수도 context 클래스 내부에 선언.

그리고 이 함수는 메인문의 루프에 넣어서 렌더 함수 실행 전에 위치시켜서 값이 들어가있을 수 있도록 해주자.


## 화면 크기 처리 리팩토링
context에 `Reshape` 함수와 `m_width`, `m_height`를 넣어주자
Reshape은 창의 크기가 변경되었을때 호출해야함.
glfwSetFramebufferSizeCallback 함수에서 세팅을 했었는데 여기서 Reshape을 진행해주자

user pointer 기능을 활용.
-> 화면 사이즈가 변해도 종횡비를 유지하며 그림이 그려지게 됨.

--- 

## 물체의 회전
- Euler angle: 물체의 회전 정보를 나타내는 기본적이고 대표적인 방식.
-> roll(z), pitch(x), yaw(y) 3개의 회전각으로 물체의 회전 정보를 나타낸다.

카메라 회전각: 카메라 회전에 roll은 보통 사용하지 않음.
-> 대신 up vector를 기준으로 roll을 설정
따라서 yaw, pitch만 가지고 camera front 방향을 결정.

회전각에 대한 정보를 담을 변수, 회전을 다룰 함수들을 context 클래스에 넣어주자.

`m_cameraPitch`, `m_cameraYaw`, `m_cameraControl`, `m_prevMousePos` 멤버 변수들을 `MouseMove`, `MouseButton` 매 렌더마다 멤버 함수들로 세팅해주자.

glfw 콜백함수를 통해 마우스, 키보드 감지 때마다 사용할 함수들을 연결해주자.

![alt text](<./opengl_example>)