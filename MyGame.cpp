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

namespace picojson
{
	template <typename T> T get_as(value& v);
	template <> Vector3 get_as<Vector3>(value& v) {
		auto& vec = v.get<array>();
		return Vector3{ float(vec[0].get<double>()), float(vec[1].get<double>()), float(vec[2].get<double>()) };
	}
	template <> Vector4 get_as<Vector4>(value& v) {
		auto& vec = v.get<array>();
		return Vector4{ float(vec[0].get<double>()), float(vec[1].get<double>()), float(vec[2].get<double>()), float(vec[3].get<double>()) };
	}
}

void MyGame::Initialize(GameContext & context)
{
	// JSON�t�@�C���̓ǂݍ���
	std::ifstream ifs("object.json");
	if (ifs.fail())
		throw std::exception("failed to read object.json");

	// JSON�f�[�^�̓ǂݍ���
	picojson::value value;
	ifs >> value;
	ifs.close();

	// �m�F�p
	//std::cout << value << std::endl;

	// �f�[�^�̎g�p
	picojson::object& root = value.get<picojson::object>();

	picojson::array& objvalues = root["objects"].get<picojson::array>();

	// �I�u�W�F�N�g�̗v�f�����ɏ���
	for (auto& objvalue : objvalues)
	{
		auto& obj = objvalue.get<picojson::object>();

		//auto& type = obj["type"].get<std::string>();
		auto position = picojson::get_as<Vector3>(obj["position"]);
		auto size = picojson::get_as<Vector3>(obj["size"]);
		auto color = picojson::get_as<Vector4>(obj["color"]);
		auto vel = Vector3::Zero;
		if (obj.count("vel"))
			vel = picojson::get_as<Vector3>(obj["vel"]);
		auto weight = 1.f;
		if (obj.count("weight"))
			weight = static_cast<float>(obj["weight"].get<double>());

		auto object = std::make_unique<CollisionObject>();
		object->Initialize(context);
		object->m_objectPos = position;
		object->m_objectColor = color;
		object->m_objectVel = vel;
		object->m_objectSize = size;
		object->m_objectWeight = weight;
		m_objects.emplace_back(std::move(object));
	}
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

	// �I�u�W�F�N�g�̍X�V
	for (auto& obj : m_objects)
		obj->Update(context);
	for (auto& obj : m_particles)
	{
		obj->m_objectAcc = Vector3::Down * .01f;
		obj->Update(context);
	}

	for (auto& obj1 : m_objects)
		for (auto& obj2 : m_objects)
		{
			if (obj1 < obj2 && Collision::IsHit(*obj1, *obj2))
			{
				{
					// �ʒu�̍��̃x�N�g���ɑ΂��Đ����ȃx�N�g���Ɛ����ȕ����ɑ��x�x�N�g���𕪉�
					auto subpos = obj2->m_objectPos - obj1->m_objectPos;	// �ʒu�̍�
					auto subvel = obj2->m_objectVel - obj1->m_objectVel;	// ���x�̍�
					auto subposNorm = subpos;	// �ʒu�̍��P�ʃx�N�g��
					auto subvelNorm = subvel;	// ���x�̍��P�ʃx�N�g��
					subposNorm.Normalize();
					subvelNorm.Normalize();

					// �搶��Arrow.cpp
					auto rotAngle = std::acos(subposNorm.Dot(subvelNorm));
					auto rotAxis = subposNorm.Cross(subvelNorm);
					if (rotAxis.Length() == 0.0f) rotAxis = Vector3(0.0f, 0.0f, 1.0f);

					// 90�x��]���Đ����ȃx�N�g����
					auto subposVerticalNorm = Vector3::Transform(subposNorm, Quaternion::CreateFromAxisAngle(rotAxis, XMConvertToRadians(90)));
					auto subvelLength = subvel.Length();
					// sin cos�Œ���������
					auto subvelVertical = subposVerticalNorm * subvelLength * std::sin(rotAngle);
					auto subvelHorizontal = subposNorm * subvelLength * std::cos(rotAngle);

					// ���x�ɉ��Z
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

				// �p�[�e�B�N��
				for (int i = 0; i < 4; i++)
				{
					// �I�u�W�F�N�g�쐬
					auto particle = std::make_unique<CollisionObject>();
					// �I�u�W�F�N�g������
					particle->Initialize(context);
					// �I�u�W�F�N�g�̏������W�ݒ�
					particle->m_objectPos = Vector3((obj1->m_objectPos + obj2->m_objectPos) * .5f);
					// �I�u�W�F�N�g�̑傫��
					particle->m_objectSize = Vector3(.25f);
					// �I�u�W�F�N�g�̑��x
					particle->m_objectVel = Vector3((std::rand() % 100 / 100.f - .5f), (std::rand() % 100 / 100.f - .5f), (std::rand() % 100 / 100.f - .5f)) * .5f;
					// �I�u�W�F�N�g�̍X�V
					particle->Update(context);
					// �I�u�W�F�N�g�̐F�ݒ�
					particle->m_objectColor = Colors::Green;
					// �ǉ�
					m_particles.emplace_back(std::move(particle));
				}
			}
		}

	for (auto itr = m_particles.begin(); itr != m_particles.end();)
	{
		if ((*itr)->m_objectPos.y < -100.f)
		{
			(*itr)->Finalize(context);
			itr = m_particles.erase(itr);
		}
		else ++itr;
	}

	auto sizeWidth = .5f;
	auto sizeDepth = .5f;
	auto sizeHeight = .5f;
	auto width = 4.f + sizeWidth;
	auto depth = 4.f + sizeDepth;
	auto verticalCoefficient = .6f;
	auto horizontalCoefficient = .998f;
	auto friction = .998f;
	for (auto& obj : m_objects)
	{
		obj->m_objectVel.y -= .0098f;
		if (obj->m_objectPos.y <= sizeHeight)
		{
			obj->m_objectPos.y = sizeHeight;
			obj->m_objectVel.y *= -verticalCoefficient;
			obj->m_objectVel.x *= friction;
			obj->m_objectVel.z *= friction;
			if (std::abs(obj->m_objectVel.y) < .05f)
				obj->m_objectVel.y = 0.f;
		}
		if (obj->m_objectPos.x >= width)
		{
			obj->m_objectPos.x = width;
			obj->m_objectVel.x *= -horizontalCoefficient;
			if (std::abs(obj->m_objectVel.x) < .05f)
				obj->m_objectVel.x = 0.f;
		}
		if (obj->m_objectPos.x <= -width)
		{
			obj->m_objectPos.x = -width;
			obj->m_objectVel.x *= -horizontalCoefficient;
			if (std::abs(obj->m_objectVel.x) < .05f)
				obj->m_objectVel.x = 0.f;
		}
		if (obj->m_objectPos.z >= depth)
		{
			obj->m_objectPos.z = 4.5f;
			obj->m_objectVel.z *= -horizontalCoefficient;
			if (std::abs(obj->m_objectVel.z) < .05f)
				obj->m_objectVel.z = 0.f;
		}
		if (obj->m_objectPos.z <= -depth)
		{
			obj->m_objectPos.z = -depth;
			obj->m_objectVel.z *= -horizontalCoefficient;
			if (std::abs(obj->m_objectVel.z) < .05f)
				obj->m_objectVel.z = 0.f;
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
	// �I�u�W�F�N�g�̕`��
	for (auto& obj : m_objects)
		obj->Render(context);
	for (auto& obj : m_particles)
		obj->Render(context);
}

void MyGame::Finalize(GameContext & context)
{
	// �I�u�W�F�N�g�̉��
	for (auto& obj : m_objects)
		obj->Finalize(context);
	for (auto& obj : m_particles)
		obj->Finalize(context);
}
