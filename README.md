# 4학기 미니 프로젝트

## 엔진 디렉토리 분리
- Engine/Include: API 헤더 파일들. 즉, 클라이언트 사이드에서 사용될 API.
- Engine/Shaders: 쉐이더 파일들.
- Source: 엔진 소스 파일들.

## DOTO List
- WindowManager
- Timer
- Engine
- WinMain
- Resource Types: Material, Mesh, Shader, Texture
- ThreadPool
- ResourceManager: Texture, Shader, Mesh, Material
- Renderer
- PhysX
- GameFramework
	- Components
		- SceneComponent
		- MeshComponent
- Animation
- Animation Blending

## CODING CONVENTIONS
```cpp
// 기본 포맷은 google-style
// 글로벌 변수 절대 X
// Tab은 알아서

// 매크로는 대문자 + _
#define MY_MACRO(x)

// 네임스페이스는 소문자
namespace foo {
// 안에 텝 X
enum class Color {  // enum class 변수는 상수처럼
  kRed, 
  kGreen, 
  kBlue,
  kMyColor
};
}
// 클래스 네임 Pascal
class MyClass {
  // 상수 대문자로 사용시 메크로랑 겹치는 문제 해결하기 위해서
  const int kMyConstVar;  // 상수는 k + Pascal
  const int kDate_0_1;    // _ 로 숫자 분리 가능
  static constexpr int kMyConstexprVar;

  int _myPrivateVariable;            // 프라이빗 변수는 _camelCase
  void MyPrivateFunction() {    // 프라이빗 함수는 PascalCase
    int a{0};  // 로컬 변수는 본인 편한대로
    return;
  }

 public:
  int myPublicVariable;  // 퍼블릭 변수는 camelCase
  // 포인터는 필요시 이름에 명시
  OtherObject *otherObj, *pOtherObj;
  void Foo(void** pVar);
  // 참조(Reference)(`&`)는 참조라고 이름에 명시 안함
  OtherObject& otherObj;

  // 기본 변수 이니셜라이징(초기화)은 Uniform 이니셜라이져 `{}` 사용
  MyClass() : _myPrivateVariable{0}, myPublicVariable{0}, pOtherObj{nullptr} {}
  // 이니셜라이징 리스트가 많으면 Ctrl+K, Ctrl+F 자동 포멧
  MyClass(int myPrivateVar, int myPublicVar, OtherObject* pObj,
          OtherObject& rObj)
      : _myPrivateVariable{myPrivateVar},
        myPublicVariable{myPublicVar},
        pOtherObj{pObj},
        rOtherObj{rObj} {}

  // 퍼블릭 함수는 PascalCase
  void MyPublicFunction() {
    int a{0};  // 로컬 변수는 본인 편한대로!
    return;
  }

  // 함수 파라미터가 많은면 Ctrl+K, Ctrl+F
  bool MyFunctionWithLongParameterList(int a, double d, Object* pObj,
                                       void* pReserved) {
    int localA{a};        // 로컬 변수는 본인 편한대로
    double local_d { d }  // 로컬 변수는 본인 편한대로

    // 라인이 길어지면 Ctrl+K, Ctrl+F
    return (a == 1) && (d == 0.7) && (pObj != nullptr) &&
           (pReserved == nullptr);
  }
};
```
