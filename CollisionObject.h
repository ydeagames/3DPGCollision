#pragma once

class GameContext;

class CollisionObject
{
public:
	static std::unique_ptr<DirectX::GeometricPrimitive>	s_pObject;		// �I�u�W�F�N�g

public:
	DirectX::SimpleMath::Matrix						m_objectMatrix;		// �I�u�W�F�N�g�̃��[���h�s��
	DirectX::SimpleMath::Vector3					m_objectPos;		// �I�u�W�F�N�g�̍��W
	DirectX::SimpleMath::Vector3					m_objectSize = DirectX::SimpleMath::Vector3(1);		// �I�u�W�F�N�g�̑傫��
	DirectX::SimpleMath::Vector3					m_objectVel;		// �I�u�W�F�N�g�̑��x
	DirectX::SimpleMath::Vector3					m_objectAcc;		// �I�u�W�F�N�g�̉����x
	DirectX::SimpleMath::Vector4					m_objectColor;		// �I�u�W�F�N�g�̐F
	DirectX::BoundingBox							m_objectCollider;	// �I�u�W�F�N�g�̓����蔻��
	bool											m_objectWireframe;	// �I�u�W�F�N�g�����C���[�t���[����

public:
	// ����
	void Initialize(GameContext& context);
	// �X�V
	void Update(GameContext& context);
	// �`��
	void Render(GameContext& context, const DirectX::SimpleMath::Vector4& color = DirectX::SimpleMath::Vector4(DirectX::Colors::White));
	// �j��
	void Finalize(GameContext& context);
};

