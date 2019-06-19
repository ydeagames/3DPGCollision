#include "pch.h"
#include "MyGame.h"
#include "GameCamera.h"
#include "GameContext.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void MyGame::Initialize(GameContext & context)
{
	// �I�u�W�F�N�g�쐬
	m_pObject1 = std::make_unique<CollisionObject>();
	m_pObject2 = std::make_unique<CollisionObject>();

	// �I�u�W�F�N�g������
	m_pObject1->Initialize(context);
	m_pObject2->Initialize(context);

	// �I�u�W�F�N�g�̏������W�ݒ�
	m_pObject1->m_objectPos = SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
	m_pObject2->m_objectPos = SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);

	// �I�u�W�F�N�g�̐F�ݒ�
	m_pObject1->m_objectColor = Colors::Red;
	m_pObject2->m_objectColor = Colors::Blue;
}

void MyGame::Update(GameContext & context)
{
	auto& key = Keyboard::Get().GetState();
	if (key.A)
		m_pObject1->m_objectPos.x -= .1f;
	if (key.D)
		m_pObject1->m_objectPos.x += .1f;
	if (key.W)
		m_pObject1->m_objectPos.z -= .1f;
	if (key.S)
		m_pObject1->m_objectPos.z += .1f;

	// �I�u�W�F�N�g�̍X�V
	m_pObject1->Update(context);
	m_pObject2->Update(context);

	if (Collision::IsHit(*m_pObject1, *m_pObject2))
	{
		auto sub = m_pObject1->m_objectPos - m_pObject2->m_objectPos;
		sub.Normalize();
		sub *= 2.f;
		m_pObject1->m_objectPos += sub;
		m_pObject2->m_objectPos -= sub;
	}
}

void MyGame::Render(GameContext & context)
{
	// �I�u�W�F�N�g�̕`��
	m_pObject1->Render(context);
	m_pObject2->Render(context);
}

void MyGame::Finalize(GameContext & context)
{
	// �I�u�W�F�N�g�̉��
	m_pObject1->Finalize(context);
	m_pObject2->Finalize(context);
}
