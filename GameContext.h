#pragma once
#include "StepTimer.h"
#include "DeviceResources.h"

class GameObject;
class GameCamera;

// GameContextインターフェイス
class GameContext
{
public:
	GameContext() = default;
	virtual ~GameContext() = default;

public:
	// DeviceResource取得
	virtual DX::DeviceResources& GetDR() = 0;
	// タイマー取得
	virtual DX::StepTimer& GetTimer() = 0;
	// カメラ取得
	virtual GameCamera& GetCamera() = 0;
	// コモンステート取得
	virtual DirectX::CommonStates& GetStates() = 0;
};
