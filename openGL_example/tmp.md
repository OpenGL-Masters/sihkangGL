# GLSL

openGL Shader Language
쉐이더: GPU 상에서 동작하는 그림을 그리기 위한 작은 프로그램
정점별 / 픽셀 별 병렬 수행되어 성능을 높임
GLSL: openGL에서 shader를 작성하기 위해 제공하는 c 기반 언어
그 외의 대표적인 쉐이더 언어: HLSL(DirectX), Metal(metal), cg(nVidia가 제시한 쉐이더 언어, unity3D에서 사용)

## 대략적인 GLSL 구조

```c
#version version_number // predifine macro

in type in_vaiable_name;

out type out_variable_name;

uniform type uniform_name;

void main(){
	// process input(s) and do some weird graphics stuff...
	// output processed stuff to output variable
	out_variable_name = weird_stuff_we_processed;
}
```

in out uniform <- 한정자. 변수가 어떤 형태로 사용되는지 정보 제공.
in: 쉐이더 프로그램의 인풋
out: 쉐이더 프로그램의 아웃풋
uniform: 항상 동일한 값으로 존재하는 const 같은 녀석

또한 void main() {} 을 엔트리 포인트로 가진다. 여기서 아웃풋으로 나갈 변수에 값을 담아주어 결과를 출력하자
vs, fs 모두 이런 형식으로 구성된다.

## GLSL data type
int, float, double, uint, bool
* 벡터 타입: vexX(float형 벡터), bvecX(bool형 벡터), ivexX(int형 벡터), uvexX(uint형 벡터), dvecX(double형 벡터)
X에는 2, 3, 4 사용 가능

* 행렬 타입: matX, bmatX, imatX, umatX, dmatX
동일하게 X에는 2, 3, 4 사용 가능. 위와 동일하게 기본자료형에 해당하는 행렬 타입이 존재.

### vector
- 벡터 원소 접근
: .x / .y / .z / .w 인덱스로 각 벡터의 성분에 접근
swizzling 가능. 얻어오고 싶은 인덱스를 연속으로 쓰기 가능 (.xyz => vec3 으로 리턴)
이러한 swizzling 기능은 .rgba, .stpq에도 동일한 방식으로 적용됨.

```
vec2 someVec;
vec4 differentVec = someVec.xyxx;
vec3 anotherVec = differentVec.zyx;
vec4 otherVec = someVec.xxxx + anotherVec.yxzy;
```
다양한 형식의 swizzling으로 편하게 벡터 표현이 가능하다.

- 벡터 초기값 선언
```
vec2 vect = vec2(0.5, 0.7); // vec2생성자 사용
vec4 result = vec4(vect, 0.0, 0.0); // 다른 벡터를 섞어서 사용 가능
vec4 otherResult = vec4(result.xyz, 1.0) // 다른 벡터 swizzling + 섞어서 사용 

```
단 기본 자료형이 일치하는 (위에서는 float 값) 내에서 섞어서 사용 가능.

## IN / OUT
쉐이더의 인풋과 아웃풋이 무엇인지를 지정.
모든 쉐이더는 용도에 맞는 입출력이 선언되어 있어야함.
in, out: 쉐이더의 입출력을 가리키는 type qualifier.

### Vertex shader
각 정점 별로 설정된 vertex attribute를 입력받는다.
몇 번째 attribute를 입력받을지에 대한 추가적인 설정을 할 수 있음.

```
layout (location = n )
```

**반드시 정점의 출력 위치 gl_Position 값을 계산해줘야함.**
이게 없으면 vs가 컴파일 되지 않는다.

- Rasterization: vertex shader의 출력값을 primitive에 맞게 보간하여 픽셀별 값으로 변환 (Rasterization)
- Fragement shader: Rasterization을 거쳐 픽셀 별로 할당된 vertex shader 의 출력값이 입력됨. 각 픽셀의 실제 색상 값이 출력되어야함.

주의해야할 점은, vs의 아웃풋과 fs의 인풋은 동일해야함.
vs -> out vec4 vertexColor
그러면 fs -> in vec4 vertexColor 로 존재해야함.
안그러면 링크에러 발생

gl_Position은 이미 기본적으로 설정된 변수라 안가져와도 되지만.

uniform: 쉐이더에 전달 가능한 global value
병렬로 수행되는 모든 쉐이더 쓰레드들이 동일한 값을 전달받는다.

변수 선언 앞에 uniform type qualifier를 사용하여 선언.

uniform variable 에 값을 입력하려면,
glGetUniformLocation() 함수를 활용하여 program object로부터 uniform handle을 얻는다. 이후 program을 바인딩 한 후 **glUniform@@** 함수를 활용하여 입력

C++ 코드에서 그림을 그릴 때 uniform이라는 공통된 값들을 설정해준다.
텍스쳐, 트랜스폼 메트릭스 등을 설정할때 사용하게 될 것이다.

## vertex attributes
하나의 버텍스가 가지는 정보는 여러가지이다.
: position, normal, tangent, color, texture coordinates, ...
각각이 하나의 버텍스 속성이 된다.

컬러 attribute를 넣어보자.

