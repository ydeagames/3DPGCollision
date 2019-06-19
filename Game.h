//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "GameContext.h"
#include "GameCamera.h"
#include "MyGame.h"

class DebugCamera;
class GridFloor;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify, public GameContext
{
public:

	Game() noexcept(false);
	~Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	// Device resources.
	std::unique_ptr<DX::DeviceResources>    m_deviceResources;

	// Rendering loop timer.
	DX::StepTimer                           m_timer;

	// マウス
	std::unique_ptr<DirectX::Mouse>			m_pMouse;
	// キーボード
	std::unique_ptr<DirectX::Keyboard>		m_pKeyboard;

	// デバッグカメラ
	DebugCamera*							m_pDebugCamera;
	// グリッド床
	GridFloor*								m_pGridFloor;
	// コモンステート
	std::unique_ptr<DirectX::CommonStates>	m_pState;

	// カメラオブジェクト
	GameCamera								m_camera;
	// ロジック
	std::unique_ptr<MyGame>					m_myGame;

	// DeviceResource取得
	DX::DeviceResources& GetDR()
	{
		return *m_deviceResources;
	}
	// タイマー取得
	DX::StepTimer& GetTimer()
	{
		return m_timer;
	}
	// カメラ取得
	GameCamera& GetCamera()
	{
		return m_camera;
	}
	// コモンステート取得
	DirectX::CommonStates& GetStates()
	{
		return *m_pState;
	}
};