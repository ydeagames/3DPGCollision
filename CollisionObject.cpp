#include "pch.h"
#include "CollisionObject.h"
#include "GameCamera.h"
#include "GameContext.h"

using namespace DirectX;

void CollisionObject::Initialize(GameContext & context)
{
	// オブジェクト作成
	m_pObject = GeometricPrimitive::CreateBox(context.GetDR().GetD3DDeviceContext(), m_objectSize);
}

void CollisionObject::Update(GameContext & context)
{
	// オブジェクトの更新
	m_objectMatrix = SimpleMath::Matrix::CreateTranslation(m_objectPos);
	// オブジェクトの当たり判定
	m_objectCollider = BoundingBox(m_objectPos, m_objectSize * .5f);
}

void CollisionObject::Render(GameContext & context)
{
	// オブジェクトの描画
	m_pObject->Draw(m_objectMatrix, context.GetCamera().view, context.GetCamera().projection, m_objectColor);
}

void CollisionObject::Finalize(GameContext & context)
{
	// オブジェクトの解放
	m_pObject.reset();
}
