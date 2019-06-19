#pragma once

class GameContext;

class CollisionObject
{
public:
	std::unique_ptr<DirectX::GeometricPrimitive>	m_pObject;			// �I�u�W�F�N�g
	DirectX::SimpleMath::Matrix						m_objectMatrix;		// �I�u�W�F�N�g�̃��[���h�s��
	DirectX::SimpleMath::Vector3					m_objectPos;		// �I�u�W�F�N�g�̍��W
	DirectX::SimpleMath::Vector3					m_objectSize = DirectX::SimpleMath::Vector3(1);		// �I�u�W�F�N�g�̑傫��
	DirectX::SimpleMath::Vector4					m_objectColor;		// �I�u�W�F�N�g�̐F
	DirectX::BoundingBox							m_objectCollider;	// �I�u�W�F�N�g�̓����蔻��

public:
	// ����
	void Initialize(GameContext& context);
	// �X�V
	void Update(GameContext& context);
	// �`��
	void Render(GameContext& context);
	// �j��
	void Finalize(GameContext& context);
};

