#include "pch.h"
#include "CollisionObject.h"
#include "GameCamera.h"
#include "GameContext.h"

using namespace DirectX;

void CollisionObject::Initialize(GameContext & context)
{
	// �I�u�W�F�N�g�쐬
	m_pObject = GeometricPrimitive::CreateBox(context.GetDR().GetD3DDeviceContext(), m_objectSize);
}

void CollisionObject::Update(GameContext & context)
{
	// �I�u�W�F�N�g�̍X�V
	m_objectMatrix = SimpleMath::Matrix::CreateTranslation(m_objectPos);
	// �I�u�W�F�N�g�̓����蔻��
	m_objectCollider = BoundingBox(m_objectPos, m_objectSize * .5f);
}

void CollisionObject::Render(GameContext & context)
{
	// �I�u�W�F�N�g�̕`��
	m_pObject->Draw(m_objectMatrix, context.GetCamera().view, context.GetCamera().projection, m_objectColor);
}

void CollisionObject::Finalize(GameContext & context)
{
	// �I�u�W�F�N�g�̉��
	m_pObject.reset();
}
