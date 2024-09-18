#include "context.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

void OnFramebufferSizeChange(GLFWwindow *window, int width, int height)
{
	SPDLOG_INFO("framebuffer size changed: ({} x {})", width, height);

	auto context = static_cast<Context*>(glfwGetWindowUserPointer(window));
	context->Reshape(width, height);
}

void OnCursorPos(GLFWwindow* window, double x, double y)
{
	auto context = (Context *)glfwGetWindowUserPointer(window);
	context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int modifier)
{
	auto context = (Context*)glfwGetWindowUserPointer(window);
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	context->MouseButton(button, action, x, y);
}

void OnKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
			key, scancode,
			action == GLFW_PRESS ? "Pressed" : 
			action == GLFW_RELEASE ? "Released" : 
			action == GLFW_REPEAT ? "Repeat" : "Unknown",
			mods & GLFW_MOD_CONTROL ? "C" : "-",
			mods & GLFW_MOD_SHIFT ? "S" : "-",
			mods & GLFW_MOD_ALT ? "A" : "-");
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	SPDLOG_INFO("Start program");

	// glfwInit()으로 glfw 라이브러리 초기화, 실패 시 에러 출력 후 종료
	SPDLOG_INFO("Initialize glfw");
	if (!glfwInit())
	{
		const char * description = nullptr;
		glfwGetError(&description); // 에러 사유를 char * 타입으로 얻어옴.
		SPDLOG_ERROR("failed to initialize glfw: {}", description); // FMT formatting 문법. {} 에 추가 인자(description) 값이 들어가게 됨.
		return -1;
	}

	// GLFW 윈도우를 생성하기 전 만들기 희망하는 오픈지엘 버전 추가
	// 앞으로 만들려하는 윈도우의 오픈지엘을 어떻게 세팅할건지 정보를 입력.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // openGL 3.3 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // opengl profile을 core profile로 설정
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // macOS에서 필요할 수 있음 deprecated된 기능 사용 방지.


	// glfw 윈도우 생성, 실패 시 에러 출력 후 종료
	SPDLOG_INFO("Create glfw window");
	auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr); 
	if (!window)
	{
		SPDLOG_ERROR("failed to create glfw window");
		const char * description = nullptr;
		glfwGetError(&description); // 에러 사유를 char * 타입으로 얻어옴.
		SPDLOG_ERROR("failed to initialize glfw: {}", description); // FMT formatting 문법. {} 에 추가 인자(description) 값이 들어가게 됨.
		glfwTerminate();
		return -1;
	}
	// glfwCreateWindow로 윈도우가 생성되고 컨텍스트가 생성됨. 여러 개의 윈도우가 생성될 수 있기 때문에, 어떤 컨텍스트에 작업을 할 건지 컨텍스트를 선택
	// 현재 작업할 윈도우 컨텍스트를 아까 위에서 생성한 windㄴow로 하겠다는 의미.
	glfwMakeContextCurrent(window);

	// glad 를 통한 openGL 함수 로딩 by gladLoadGLLoader. 인자로 들어가는 녀석은 프로세스 주소를 받아오는 함수.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		SPDLOG_ERROR("failed to initialize glad");
		glfwTerminate();
		return -1;
	}
	// 이제 gl로 시작하는 opengl 함수를 쓸 수 있다. 
	// 버젼 확인 시 4.1로 나오는데 이는 맥os의 하드웨어가 4.1을 지원함. glVersion은 현재 컨텍스트에서 지원되는 오픈지엘 버젼 반환. 따라서 4.1이 나옴.
	// 4.1을 지원하나 내가 3.3을 요청해서 그에 해당하는 버전에 맞추어 사용하게됨.
	auto glVersion = glGetString(GL_VERSION);
	SPDLOG_INFO("OpenGL context version: {}", reinterpret_cast<const char*>(glVersion));

	auto context = Context::Create();
	if (!context)
	{
		SPDLOG_ERROR("failed to create context");
		glfwTerminate();
		return -1;
	}
	
	// 쌩포인터를 윈도우에 유저 포인터로서 세팅해주기.->reshape 을 위해
	glfwSetWindowUserPointer(window, context.get());

	// 윈도우 생성 직후에는 프레임 버퍼 변경 이벤트가 발생하지 않으므로 첫 호출은 수동으로 함.
	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// 콜백함수를 윈도우에 등록
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);
	glfwSetCursorPosCallback(window, OnCursorPos);
	glfwSetMouseButtonCallback(window, OnMouseButton);

	// glfw loop 실행. 윈도우 닫기 버튼 누르면 정상 종료
	SPDLOG_INFO("Start main loop");

	// FRAMEBUFFER SWAP (더블 버퍼링, 트리플 버퍼링 등 ... ) 버퍼가 많을 수록 그림을 그릴 여유가 있어 부드러운 그림을 그릴 ㅜㅅ있다.
	// 화면에 그림을 그리는 과정으로, 프레임버퍼를 front/ back 두 개를 준비하고 front는 화면에 노출, 그동안 back에 그림을 그리고, 다음 그림은 back를 노출.
	// front <-> back 바꿔치는 과정을 반복하며 그림그리는 과정이 노출되지 않고 깔끔하게 변하도록 해줌.
	// 요즘 하드웨어에서 싱글 버퍼로 그리는 경우는 없다.

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 윈도우와 관련된 키보드, 마우스 이벤트 등을 감지하는. poll() 과 유사한 기능.
		context->ProcessInput(window);
		context->Render();
		glfwSwapBuffers(window);
	}
	context.reset();
	
	glfwTerminate();
	return 0;
}