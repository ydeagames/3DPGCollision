#include "pch.h"
#include "CollisionObject.h"
#include "GameCamera.h"
#include "GameContext.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<DirectX::GeometricPrimitive> CollisionObject::s_pObject;

void CollisionObject::Initialize(GameContext & context)
{
	// オブジェクト作成
	if (!s_pObject)
		s_pObject = GeometricPrimitive::CreateCube(context.GetDR().GetD3DDeviceContext());
}

void CollisionObject::Update(GameContext & context)
{
	// オブジェクトの物理
	m_objectVel += m_objectAcc;
	m_objectVel *= .98f;
	m_objectPos += m_objectVel;
	// オブジェクトの更新
	m_objectMatrix = Matrix::CreateScale(m_objectSize) * Matrix::CreateTranslation(m_objectPos);
	// オブジェクトの当たり判定
	m_objectCollider = BoundingBox(m_objectPos, m_objectSize * .5f);
}

void CollisionObject::Render(GameContext & context, const DirectX::SimpleMath::Vector4& color)
{
	// オブジェクトの描画
	s_pObject->Draw(m_objectMatrix, context.GetCamera().view, context.GetCamera().projection, m_objectColor * color, false, m_objectWireframe);
}

void CollisionObject::Finalize(GameContext & context)
{
}
