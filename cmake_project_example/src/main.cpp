#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

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

	// glfw 윈도우 생성, 실패 시 에러 출력 후 종료
	SPDLOG_INFO("Create glfw window");
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr); 
	if (!window)
	{
		SPDLOG_ERROR("failed to create glfw window");
		glfwTerminate();
		return -1;
	}

	// glfw loop 실행. 윈도우 닫기 버튼 누르면 정상 종료
	SPDLOG_INFO("Start main loop");
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); // 윈도우와 관련된 키보드, 마우스 이벤트 등을 감지하는. poll() 과 유사한 기능.
	}

	glfwTerminate();
	return 0;
}