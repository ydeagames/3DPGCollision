#pragma once
#include "CollisionObject.h"

class GameContext;

class MyGame
{
private:
	// �I�u�W�F�N�g�֘A
	std::vector<std::unique_ptr<CollisionObject>> m_objects;	// �I�u�W�F�N�g

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureDiffuse;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureRoughness;

	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::GeometricPrimitive> m_plane;
	// �G�t�F�N�g
	std::unique_ptr<DirectX::PBREffect> m_basicEffect;
	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

	DirectX::SimpleMath::Vector3 lastMouse;
	float yaw = 0.f;
	float pitch = 0.f;
	
public:
	// ����
	void Initialize(GameContext& ctx);
	// �X�V
	void Update(GameContext& context);
	// �`��
	void Render(GameContext& context);
	// �j��
	void Finalize(GameContext& context);
};

