# Repository Guidelines

## Project Structure & Modules

- `Source/BG3`: 게임 코드(C++). `Public/` 헤더, `Private/` 소스
- `Content/`: 에셋(메시, 머티리얼, 블루프린트 등)
- `Config/`: 엔진/프로젝트 설정
- `Tests/` (옵션): Automation/Functional 테스트

## Build & Run

- IDE 빌드: Rider에서 프로젝트 열고 `Build → Rebuild`
- 명령행(에디터 타겟 예시):

  ```bat
  "<UE>\Engine\Build\BatchFiles\Build.bat" BG3Editor Win64 Development -Project="<PATH>\BG3.uproject" -WaitMutex
  ```

- 실행: 언리얼 에디터에서 레벨 열고 Play
- 핫 리로드: C++ 변경 시 `Compile` 또는 Rider 빌드

## Coding Style & Naming

- UE 스타일: `UCLASS`, `USTRUCT`, `UFUNCTION` 매크로 사용. 타입/클래스는 PascalCase, 변수는 camelCase. `UPROPERTY`/`UPARAM` 메타 유지
- 폴더 네임스페이스: `Source/BG3/<Domain>/...` (예: `Component`, `AI`, `UI`)
- 로그: `DECLARE_LOG_CATEGORY_EXTERN` / `DEFINE_LOG_CATEGORY`로 모듈별 카테고리 정의

## Testing

- Automation: `AutomationTest.h` 기반 단위 테스트 권장
- 이름 규칙: 파일명 `*Tests.cpp`, 테스트명 `Project.Feature.Scenario`
- 실행: 에디터 `Session Frontend → Automation`

## Git & PR

- 브랜치: `feat/`, `fix/`, `chore/` 접두사
- 커밋 메시지: `type(scope): summary` (예: `fix(skill): null check in AddSkill`)
- PR 요구사항: 변경 요약, 스크린샷/로그(필요 시), 관련 이슈 링크, 테스트 결과

