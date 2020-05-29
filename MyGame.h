#pragma once
#include "CollisionObject.h"

class GameContext;

class MyGame
{
private:
	// オブジェクト関連
	std::vector<std::unique_ptr<CollisionObject>> m_objects;	// オブジェクト

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureDiffuse;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureRoughness;

	// プリミティブバッチ
	std::unique_ptr<DirectX::GeometricPrimitive> m_plane;
	// エフェクト
	std::unique_ptr<DirectX::PBREffect> m_basicEffect;
	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	DirectX::SimpleMath::Vector3 lastMouse;
	float yaw = 0.f;
	float pitch = 0.f;
	
public:
	// 生成
	void Initialize(GameContext& ctx);
	// 更新
	void Update(GameContext& context);
	// 描画
	void Render(GameContext& context);
	// 破棄
	void Finalize(GameContext& context);
};

