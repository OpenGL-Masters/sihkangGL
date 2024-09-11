## Shader Code Loading
- 임의의 쉐이더 파일을 읽는다.
- 쉐이더 오브젝트를 만들고 쉐이더를 읽어들인 쉐이더 코드를 세팅
- 쉐이더 컴파일
- 쉐이더 컴파일 결과 실패 시 에러 리포팅.

우선 프로젝트가 공통적으로 사용할 기능을 정의할 src/common.h 파일을 만들어보자.

### optional <>

```
std::optional<std::string> LoadTextFile(const std::string &filename);
```

c++17 표준 라이브러리.
LoadTextFile은 파일을 읽어오는 함수인데, 파일을 읽어오지 못하는 경우가 발생할 것.
std::optional을 이용하면, 값이 들어있을 경우 정상적으로 사용 가능하지만, 값이 들어있지 않다면 안전하게 사용못하게 되는 효과.

## Shader class design
openGL Shader object를 가지고 있다.
인스턴스가 생성될 때 로딩할 파일명을 입력받자.
입력된 파일명으로부터 인스턴스 생성이 실패하면 메모리 할당 해제
C++11 smartpointer 활용.

### Smarpointer?
사용하기 좀 더 안전한 포인터.
메모리 할당을 받을 때, 소유권에 대한 정보가 있다.(RAII)
명시적인 delete가 필요 없어짐.

- std::unique_ptr<>: 해당 메모리 블록을 단독으로 소유
소유권을 가진 인스턴스가 스코프 밖으로 벗어났을 때, 메모리 자동 해제
인스턴스가 스코프를 벗어나게 되면, 할당되었던 메모리가 인스턴스 소멸자에 의해 delete 된다.

유니크 포인터는 이름대로, 유일한 소유권으로만 존재할 수 있다. 둘 이상의 소유권을 가지지 않게 설계됨.

소유권은 복사되지 않기때문에, std::move를 통해 이동의 개념을 활용하여 a에 있던 소유권을 b로 이동시켜줘야함.

- std::shared_ptr<>: 해당 메모리 블록의 소유권을 공유
- std::weak_ptr<> : 해당 메모리 소유권은 없지만 접근은 가능.

아무튼, 쉐이더 클래스를 설계해보자.
1. 생성자를 private한 이유
CreateFromFile() 함수 외에 다른 방식의 shader 인스턴스 생성을 막기 위함.

클래스를 생성할 수 있는 유일한 방법으로 static function call으로 한정한 것.

쉐이더를 생성하려면 CreateFromFile 함수를 이용하도록 하자.

2. Get()은 있는데 Set()은 없는 이유
쉐이더 오브젝트의 생성 관리는 쉐이더 내부에서만 관리
값의 노출을 최소화하기 위해.
쉐이더 오브젝트는 쉐이더 내부에서만 관리되게끔 함.

3. LoadFile()이 boolean 을 리턴하는 이유.
생성에 실패할 경우 false를 리턴하기 위해서

### OPENGL REMARKS
- glCreateShader() : openGL shader obj 생성
- glShaderSource() : shader에 소스 코드 설정
- glCompileShader() : 읽어들인 소스코드 컴파일 진행
- glGetShaderiv() : shader에 대한 정수형(i) 정보를 얻어옴.
- glGetShaderInfoLog(): shader 에 대한 로그를 얻어옴. 컴파일 에러 얻어내는 용도로 사용
- glDeleteShader() : shader object 제거.