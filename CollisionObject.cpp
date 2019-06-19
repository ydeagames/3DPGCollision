#include "pch.h"
#include "CollisionObject.h"
#include "GameCamera.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<DirectX::GeometricPrimitive> CollisionObject::s_pObject;

void CollisionObject::Initialize(GameContext & context)
{
	// �I�u�W�F�N�g�쐬
	if (!s_pObject)
		s_pObject = GeometricPrimitive::CreateCube(context.GetDR().GetD3DDeviceContext());
}

void CollisionObject::Update(GameContext & context)
{
	// �I�u�W�F�N�g�̕���
	m_objectVel += m_objectAcc;
	m_objectVel *= .98f;
	m_objectPos += m_objectVel;
	// �I�u�W�F�N�g�̍X�V
	m_objectMatrix = Matrix::CreateScale(m_objectSize) * Matrix::CreateTranslation(m_objectPos);
	// �I�u�W�F�N�g�̓����蔻��
	m_objectCollider = BoundingBox(m_objectPos, m_objectSize * .5f);
}

void CollisionObject::Render(GameContext & context, const DirectX::SimpleMath::Vector4& color)
{
	// �I�u�W�F�N�g�̕`��
	s_pObject->Draw(m_objectMatrix, context.GetCamera().view, context.GetCamera().projection, m_objectColor * color, false, m_objectWireframe);
}

void CollisionObject::Finalize(GameContext & context)
{
}
