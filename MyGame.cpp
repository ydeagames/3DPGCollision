#include "pch.h"
#include "MyGame.h"
#include "GameCamera.h"
#include "GameContext.h"
#include "Collision.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void MyGame::Initialize(GameContext & context)
{
	// オブジェクト作成
	m_pObject1 = std::make_unique<CollisionObject>();
	m_pObject2 = std::make_unique<CollisionObject>();
	m_pObjectField = std::make_unique<CollisionObject>();

	// オブジェクト初期化
	m_pObject1->Initialize(context);
	m_pObject2->Initialize(context);
	m_pObjectField->Initialize(context);

	// オブジェクトの初期座標設定
	m_pObject1->m_objectPos = Vector3(1.0f, 0.0f, 0.0f);
	m_pObject2->m_objectPos = Vector3(-1.0f, 0.0f, 0.0f);

	// オブジェクトの大きさ
	m_pObjectField->m_objectSize = Vector3(10.0f, 10.0f, 10.0f);

	// オブジェクトの色設定
	m_pObject1->m_objectColor = Colors::Red;
	m_pObject2->m_objectColor = Colors::Blue;
	m_pObjectField->m_objectColor = Colors::White;

	// オブジェクトのワイヤーフレーム
	m_pObjectField->m_objectWireframe = true;
}

void MyGame::Update(GameContext & context)
{
	auto key = Keyboard::Get().GetState();
	{
		auto input = Vector3::Zero;
		if (key.A)
			input.x -= 1;
		if (key.D)
			input.x += 1;
		if (key.W)
			input.z -= 1;
		if (key.S)
			input.z += 1;
		if (key.Q)
			input.y -= 1;
		if (key.E)
			input.y += 1;
		m_pObject1->m_objectAcc = input * .01f;
	}
	{
		auto input = Vector3::Zero;
		if (key.Left)
			input.x -= 1;
		if (key.Right)
			input.x += 1;
		if (key.Up)
			input.z -= 1;
		if (key.Down)
			input.z += 1;
		if (key.LeftShift)
			input.y -= 1;
		if (key.Space)
			input.y += 1;
		m_pObject2->m_objectAcc = input * .01f;
	}

	// オブジェクトの更新
	m_pObject1->Update(context);
	m_pObject2->Update(context);
	m_pObjectField->Update(context);
	for (auto& obj : m_particles)
	{
		obj->m_objectAcc = Vector3::Down * .01f;
		obj->Update(context);
	}

	if (Collision::IsHit(*m_pObject1, *m_pObject2))
	{
		{
			// 位置の差のベクトルに対して垂直なベクトルと水平な方向に速度ベクトルを分解
			auto subpos = m_pObject2->m_objectPos - m_pObject1->m_objectPos;	// 位置の差
			auto subvel = m_pObject2->m_objectVel - m_pObject1->m_objectVel;	// 速度の差
			auto subposNorm = subpos;	// 位置の差単位ベクトル
			auto subvelNorm = subvel;	// 速度の差単位ベクトル
			subposNorm.Normalize();
			subvelNorm.Normalize();

			// 先生のArrow.cpp
			auto rotAngle = std::acos(subposNorm.Dot(subvelNorm));
			auto rotAxis = subposNorm.Cross(subvelNorm);
			if (rotAxis.Length() == 0.0f) rotAxis = Vector3(0.0f, 0.0f, 1.0f);

			// 90度回転して垂直なベクトル軸
			auto subposVerticalNorm = Vector3::Transform(subposNorm, Quaternion::CreateFromAxisAngle(rotAxis, XMConvertToRadians(90)));
			auto subvelLength = subvel.Length();
			// sin cosで長さを加味
			auto subvelVertical = subposVerticalNorm * subvelLength * std::sin(rotAngle);
			auto subvelHorizontal = subposNorm * subvelLength * std::cos(rotAngle);

			// 速度に加算
			m_pObject1->m_objectVel = -(subvelHorizontal * -.5f + subvelVertical * .5f);
			m_pObject2->m_objectVel = +(subvelHorizontal * -.5f + subvelVertical * .5f);
			m_pObject1->m_objectPos += m_pObject1->m_objectVel;
			m_pObject2->m_objectPos += m_pObject2->m_objectVel;
		}

		// パーティクル
		for (int iz = 0; iz < 4; iz++)
			for (int iy = 0; iy < 4; iy++)
				for (int ix = 0; ix < 4; ix++)
				{
					// オブジェクト作成
					auto particle = std::make_unique<CollisionObject>();
					// オブジェクト初期化
					particle->Initialize(context);
					// オブジェクトの初期座標設定
					particle->m_objectPos = Vector3((ix - 2) * .125f, (iy - 2) * .125f, (iz - 2) * .125f) + (m_pObject1->m_objectPos + m_pObject2->m_objectPos) * .5f;
					// オブジェクトの大きさ
					particle->m_objectSize = Vector3(.25f);
					// オブジェクトの速度
					particle->m_objectVel = Vector3((std::rand() % 100 / 100.f - .5f), (std::rand() % 100 / 100.f - .5f), (std::rand() % 100 / 100.f - .5f)) * .5f;
					// オブジェクトの更新
					particle->Update(context);
					// オブジェクトの色設定
					particle->m_objectColor = Colors::Green;
					// 追加
					m_particles.emplace_back(std::move(particle));
				}
	}

	if (!Collision::IsHit(*m_pObject1, *m_pObjectField))
		m_pObject1->m_objectPos *= -.98f;
	if (!Collision::IsHit(*m_pObject2, *m_pObjectField))
		m_pObject2->m_objectPos *= -.98f;
	for (auto itr = m_particles.begin(); itr != m_particles.end();)
	{
		if (!Collision::IsHit(*(*itr), *m_pObjectField))
		{
			(*itr)->Finalize(context);
			itr = m_particles.erase(itr);
		}
		else ++itr;
	}
}

void MyGame::Render(GameContext & context)
{
	bool hitObject = Collision::IsHit(*m_pObject1, *m_pObject2);
	bool hitField = !Collision::IsHit(*m_pObject1, *m_pObjectField) || !Collision::IsHit(*m_pObject2, *m_pObjectField);
	// オブジェクトの描画
	m_pObject2->Render(context, Color(hitObject ? Colors::Yellow : Colors::White));
	m_pObject1->Render(context, Color(hitObject ? Colors::Yellow : Colors::White));
	m_pObjectField->Render(context, Color(hitField ? Colors::Red : Colors::Gray));
	for (auto& obj : m_particles)
		obj->Render(context);
}

void MyGame::Finalize(GameContext & context)
{
	// オブジェクトの解放
	m_pObject1->Finalize(context);
	m_pObject2->Finalize(context);
	for (auto& obj : m_particles)
		obj->Finalize(context);
}
