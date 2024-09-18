# C++ Matrix/vector calculation
- GLSL 의 경우 내부적으로 행렬 및 벡터와 관련된 다양한 기능 및 내부함수 제공
- C++에는 기본적인 수학적 연산 외에 선형대수 관련 기능은 제공되지 않음.
-> 라이브러리를 이용하자.

## C++ matrix library
### Eigen3
- C++ 선형대수 라이브러리로, 헤더파일로만 구성됨.
- 많은 라이브러리들이 이걸 사용. openGL 외, openCV 안에서 이미지로딩 시 eigen3로 변환.
- 일반적인 N차원 선형대수 연산에 사용되는. 사이즈가 큰 라이브러리이다.

### GLM
- oenGL Math library
- 3D graphics에 필요한 4차원 선형대수 연산까지를 지원.
- 우리가 사용할 컴팩스한 사이즈의 라이브러리.
-> Dependency.cmake에 추가해보자.

마찬가지로 헤더파일로만 구성되었으므로, 우리가 사용할 디렉토리로 카피해오는 식으로 진행해보자.

```cmake

ExternalProject_Add(
	dep_glm
	GIT_REPOSITORY "https://github.com/g-truc/glm"
	GIT_TAG "0.9.9.8"
	GIT_SHALLOW 1
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	TEST_COMMAND ""
	INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
		${DEP_INSTALL_DIR}/include/glm
	)
	
set(DEP_LIST ${DEP_LIST} dep_glm)

```

이후 common.h 파일에 glm 라이브러리 헤더를 추가해주자.

## Vertex transformation
- VBO 상의 정점은 고정.
- vertex shader에서 변환 행렬을 uniform으로 입력
- vertex shader 내에서 행렬곱 계산
- 해당 유니폼 행렬에 대한 내용은 context.cpp에서 입력하여 glGetUniformLocation, glUniformMatrix4fv 함수를 통해 쉐이더로 전달해주기.

# Coordinate System
- 어떤 정점 위치를 기술하기 위한 기준
- 선형 변환은 한 좌표계로 기술된 벡터를 다른 좌표계에 대해 기술하는 변환으로 해석할 수 있음.

## 좌표 공간 간의 변환
openGL 의 그림이 그려지는 공간
- [-1, 1] 범위로 normalized 된 공간
- Canonical space

object들은 local space 를 기준으로 기술됨.
Local space -> World space -> View space -> Canonical space 로 변환.

### Transform Matrix
model: Local -> world
View : World -> Camera
Projection : Camera -> Canonical
Clip space 에서 [-1, 1] 범위 밖으로 벗어난 면들은 clipping

## Orthogonal Projection
projection에는 크게 두가지가 있다.

### 1. 직교투영
원근감 없이 평행상 선이 계속 평행하도록 투영하는 방식
설계도면 등을 그려낼 때 유용
- 6개 파라미터: left, right, bottom, top, near, far
- z축에 -1: Clip space이후에는 오른손 좌표계에서 왼손 좌표계로 변경됨을 고려하는 것.

### 2. 원근 투영
변환 이전에 평행한 선이 변환 후에 한 점에서 만남(소실점)
멀리 있을 수록 물체가 작아져 원근감이 발생
- 4개의 파라미터: 종횡비(aspect ratio), 화각(foV), near, far

모든 변환의 조합
-> local space를 기준으로 한 좌표의 clip space 에서의 좌표는 소위 MVP(Model-View-Projection) matrix를 거쳐 구할 수 있음.

## 코드 적용하기
```
auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

auto projection = glm::perspective(glm::radians(45.0f), (float)640/(float)480, 0.01f, 10.0f);

auto transform = projection * view * model;
auto transformLoc = glGetUniformLocation(m_program->Get(), "transform");
```
적용하게 되면 기존에 나오던 이미지가 x축 기준 -55도 눕혀지고, -z방향으로 3만큼 이동한 형태의 이미지를 얻게된다.

## 큐브로 만들기
그냥 냅다 큐브로 만들면 다소 이상한 큐브가 생김
-> depth buffer(z 버퍼)를 고려해야함.

각 픽셀의 컬러값을 저장하는 버퍼 외에, 해당 픽셀의 깊이값(z축값)을 저장.

깊이 테스트
- 어떤 픽셀의 값을 업데이트 하기 전, 현재 그리려는 픽셀의 z값과 깊이 버퍼에 저장된 해당 위치의 z값을 비교해봄.
- 비교 결과 현재 그리려는 픽셀이 이전에 그려진 픽셀보다 뒤에 있을 경우 픽셀을 그리지 않음.

openGL의 depth buffer 초기값은 1
1이 가장 뒤에 있고, 0이 가장 앞을 의미(왼손 좌표계)

glEnable(GL_DEPTH_TEST)
glDisable(GL_DEPTH_TEST)로 깊이 데스트를 켜고 끌 수 있음.

glDepthFunc()를 이용해서 깊이 테스트 통과 조건을 변경할 수 있음.
깊이 테스트 통과 조건의 기본값은 GL_LESS. 더 작은 z값인 경우 그림을 그리는 것이 기본.

### context::Render() 수정
```
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glEnable(GL_DEPTH_TEST);
```
추가하여 z-buffer 테스트를 켜주자.

