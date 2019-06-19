#pragma once
#include "CollisionObject.h"

class GameContext;

class MyGame
{
private:
	// オブジェクト関連
	std::unique_ptr<CollisionObject>	m_pObject1;			// オブジェクト１
	std::unique_ptr<CollisionObject>	m_pObject2;			// オブジェクト２
	std::unique_ptr<CollisionObject>	m_pObjectField;		// オブジェクトフィールド
	std::vector<std::unique_ptr<CollisionObject>> m_particles; // パーティクル

public:
	// 生成
	void Initialize(GameContext& context);
	// 更新
	void Update(GameContext& context);
	// 描画
	void Render(GameContext& context);
	// 破棄
	void Finalize(GameContext& context);
};

