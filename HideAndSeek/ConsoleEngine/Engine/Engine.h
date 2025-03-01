﻿#pragma once

#include "Core.h"

#include <random>
#include <functional>

#include "Math/Vector2.h"

class Level;
class Actor;
class ScreenBuffer;

// 입력 처리를 위한 구조체
struct KeyState
{
	// 현재 프레임에 키가 눌렸는지 확인
	bool isKeyDown = false;
	// 이전 프레임에 키가 눌렸었는지 확인
	bool wasKeyDown = false;
};

// 엔진 클래스
class ENGINE_API Engine
{
public:
	Engine();
	virtual ~Engine();

	// 엔진 실행 함수
	void Run();

	// 레벨 추가 함수
	void LoadLevel(Level* newLevel);

    // 즉시 변경하지 않고 레벨 변경 요청만 하는 메서드
    void RequestLevelChange(Level* newLevel);

    // 액터 추가 / 삭제 함수
    void AddActor(Actor* newActor);
    void DestroyActor(Actor* targetActor);

    // 화면 좌표 관련 함수
    void SetCursorType(CursorType cursorType);

    void Draw(const Vector2& position, const char* image, Color color = Color::White);

    // 화면 크기 반환 함수
    inline Vector2 ScreenSize() const { return screenSize; }
	
	int GetRandomInt(int min, int max)
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}

    // 타겟 프레임 속도 설정 함수
    void SetTargetFrameRate(float targetFrameRate);

	// 입력 관련 함수/
	bool GetKey(int key);
	bool GetKeyDown(int key);
	bool GetKeyUp(int key);

    Vector2 MousePosition() const;

	// 엔진 종료 함수
	void QuitGame();
    void SetOnQuitCallBack(std::function<void()> onQuitCallback);

	// 싱글톤 객체 접근 함수
	static Engine& Get();

//protected:
public:
	void ProcessInput();				// 입력
    virtual void CheckInput();
	void Update(float deltaTime);		// Tick

    void Clear();                       // 화면 지우기.
	void Draw();						// Render
    void Present();

	// 이전 프레임의 키 상태를 저장하는 함수
	void SavePreviousKeyStates();

    inline ScreenBuffer* GetRenderer() const { return renderTargets[currentRenderTargetIndex]; }
    void ClearImageBuffer();

protected:

    // 타겟 프레임 변수 (초당 프레임)
    float targetFrameRate = 60.0f;
    
    // 한 프레임 시간 값 (초 단위)
    float targetOneFrameTime = 0.0f;

	// 종료할 때 설정할 변수
	bool quit;

	// 키 상태를 저장하는 배열
	KeyState keyState[255];

	// 싱글톤 구현을 위한 전역 변수 선언
	static Engine* Instance;

	// 메인 레벨 변수
	Level* mainLevel;

    // 로드 대기 중인 레벨
    Level* pendingLevel; 
    bool levelChangeRequested;

    // 화면 크기
    Vector2 screenSize;

    // 마우스 좌표 위치.
    Vector2 mousePosition;

    // 화면 버퍼(Buffer/Blob).
    CHAR_INFO* imageBuffer = nullptr;

    // 화면 버퍼.
    ScreenBuffer* renderTargets[2];
    int currentRenderTargetIndex = 0;

    // 종료 시 호출할 콜백 함수.
    std::function<void()> onQuitCallback; 
};