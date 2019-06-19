#pragma once

class GameContext;

class CollisionObject
{
public:
	static std::unique_ptr<DirectX::GeometricPrimitive>	s_pObject;		// オブジェクト

public:
	DirectX::SimpleMath::Matrix						m_objectMatrix;		// オブジェクトのワールド行列
	DirectX::SimpleMath::Vector3					m_objectPos;		// オブジェクトの座標
	DirectX::SimpleMath::Vector3					m_objectSize = DirectX::SimpleMath::Vector3(1);		// オブジェクトの大きさ
	DirectX::SimpleMath::Vector3					m_objectVel;		// オブジェクトの速度
	DirectX::SimpleMath::Vector3					m_objectAcc;		// オブジェクトの加速度
	DirectX::SimpleMath::Vector4					m_objectColor;		// オブジェクトの色
	DirectX::BoundingBox							m_objectCollider;	// オブジェクトの当たり判定
	bool											m_objectWireframe;	// オブジェクトがワイヤーフレームか

public:
	// 生成
	void Initialize(GameContext& context);
	// 更新
	void Update(GameContext& context);
	// 描画
	void Render(GameContext& context, const DirectX::SimpleMath::Vector4& color = DirectX::SimpleMath::Vector4(DirectX::Colors::White));
	// 破棄
	void Finalize(GameContext& context);
};

