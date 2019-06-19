#pragma once

class GameContext;

class CollisionObject
{
public:
	std::unique_ptr<DirectX::GeometricPrimitive>	m_pObject;			// オブジェクト
	DirectX::SimpleMath::Matrix						m_objectMatrix;		// オブジェクトのワールド行列
	DirectX::SimpleMath::Vector3					m_objectPos;		// オブジェクトの座標
	DirectX::SimpleMath::Vector3					m_objectSize = DirectX::SimpleMath::Vector3(1);		// オブジェクトの大きさ
	DirectX::SimpleMath::Vector4					m_objectColor;		// オブジェクトの色
	DirectX::BoundingBox							m_objectCollider;	// オブジェクトの当たり判定

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

