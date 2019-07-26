#include "pch.h"
#include "MyGame.h"
#include "GameCamera.h"
#include "GameContext.h"
#include "Collision.h"
#include "Input.h"
#include <fstream>
#include <sstream>
#include "picojson.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void MyGame::Initialize(GameContext & context)
{
	// オブジェクトの処理
	auto object = std::make_unique<CollisionObject>();
	object->Initialize(context);
	object->m_objectPos = Vector3::Zero;
	object->m_objectColor = Colors::White;
	object->m_objectVel = Vector3::Zero;
	object->m_objectSize = Vector3::One;
	object->m_objectWeight = 1;
	m_objects.emplace_back(std::move(object));
}

void MyGame::Update(GameContext & context)
{
	Input::Update();

	auto key = Keyboard::Get().GetState();
	{
		auto input = Vector3::Zero;
		if (key.A || key.Left)
			input.x -= 1;
		if (key.D || key.Right)
			input.x += 1;
		if (key.W || key.Up)
			input.z -= 1;
		if (key.S || key.Down)
			input.z += 1;
		if (key.Q || key.LeftShift)
			input.y -= 1;
		if (key.E || key.Space)
			input.y += 1;
		m_objects[0]->m_objectAcc = input * .01f;
	}

	// オブジェクトの更新
	for (auto& obj : m_objects)
		obj->Update(context);

	for (auto& obj1 : m_objects)
		for (auto& obj2 : m_objects)
		{
			if (obj1 < obj2 && Collision::IsHit(*obj1, *obj2))
			{
				{
					// 位置の差のベクトルに対して垂直なベクトルと水平な方向に速度ベクトルを分解
					auto subpos = obj2->m_objectPos - obj1->m_objectPos;	// 位置の差
					auto subvel = obj2->m_objectVel - obj1->m_objectVel;	// 速度の差
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
					auto w = obj1->m_objectWeight + obj2->m_objectWeight;
					auto w1 = obj1->m_objectWeight / w;
					auto w2 = obj2->m_objectWeight / w;
					obj1->m_objectVel = -(subvelHorizontal * -w2 + subvelVertical * .5f);
					obj2->m_objectVel = +(subvelHorizontal * -w1 + subvelVertical * .5f);
					//obj1->m_objectPos += obj1->m_objectVel;
					//obj2->m_objectPos += obj2->m_objectVel;
					auto radius = obj1->m_objectCollider.Radius + obj2->m_objectCollider.Radius;
					auto subradius = subpos.Length() - radius;
					auto subadd = subposNorm * subradius * .55f;
					obj1->m_objectPos += +subadd;
					obj2->m_objectPos += -subadd;
				}
			}
		}

	if (Input::GetMouseButtonDown(Input::Buttons::MouseRight))
	{
		auto ray = context.GetCamera().ScreenPointToRay(Input::GetMousePosition());
		std::unique_ptr<CollisionObject>* nearest = nullptr;
		float nearestDist = 1.e+9f;
		for (auto& obj : m_objects)
		{
			float dist;
			if (ray.Intersects(obj->m_objectCollider, dist))
			{
				if (!nearest || nearestDist > dist)
				{
					nearest = &obj;
					nearestDist = dist;
				}
			}
		}
		if (nearest)
		{
			auto raypos = ray.position + ray.direction * nearestDist;
			auto sub = (*nearest)->m_objectCollider.Center - raypos;
			auto subNorm = sub;
			subNorm.Normalize();
			(*nearest)->m_objectVel += subNorm;
		}
	}
}

void MyGame::Render(GameContext & context)
{
	// オブジェクトの描画
	for (auto& obj : m_objects)
		obj->Render(context);
}

void MyGame::Finalize(GameContext & context)
{
	// オブジェクトの解放
	for (auto& obj : m_objects)
		obj->Finalize(context);
}
