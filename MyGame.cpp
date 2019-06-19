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
	m_pObjectField = std::make_unique<CollisionObject>();

	// �I�u�W�F�N�g������
	m_pObject1->Initialize(context);
	m_pObject2->Initialize(context);
	m_pObjectField->Initialize(context);

	// �I�u�W�F�N�g�̏������W�ݒ�
	m_pObject1->m_objectPos = Vector3(1.0f, 0.0f, 0.0f);
	m_pObject2->m_objectPos = Vector3(-1.0f, 0.0f, 0.0f);

	// �I�u�W�F�N�g�̑傫��
	m_pObjectField->m_objectSize = Vector3(10.0f, 10.0f, 10.0f);

	// �I�u�W�F�N�g�̐F�ݒ�
	m_pObject1->m_objectColor = Colors::Red;
	m_pObject2->m_objectColor = Colors::Blue;
	m_pObjectField->m_objectColor = Colors::White;

	// �I�u�W�F�N�g�̃��C���[�t���[��
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

	// �I�u�W�F�N�g�̍X�V
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
			// �ʒu�̍��̃x�N�g���ɑ΂��Đ����ȃx�N�g���Ɛ����ȕ����ɑ��x�x�N�g���𕪉�
			auto subpos = m_pObject2->m_objectPos - m_pObject1->m_objectPos;	// �ʒu�̍�
			auto subvel = m_pObject2->m_objectVel - m_pObject1->m_objectVel;	// ���x�̍�
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
			m_pObject1->m_objectVel = -(subvelHorizontal * -.5f + subvelVertical * .5f);
			m_pObject2->m_objectVel = +(subvelHorizontal * -.5f + subvelVertical * .5f);
			m_pObject1->m_objectPos += m_pObject1->m_objectVel;
			m_pObject2->m_objectPos += m_pObject2->m_objectVel;
		}

		// �p�[�e�B�N��
		for (int iz = 0; iz < 4; iz++)
			for (int iy = 0; iy < 4; iy++)
				for (int ix = 0; ix < 4; ix++)
				{
					// �I�u�W�F�N�g�쐬
					auto particle = std::make_unique<CollisionObject>();
					// �I�u�W�F�N�g������
					particle->Initialize(context);
					// �I�u�W�F�N�g�̏������W�ݒ�
					particle->m_objectPos = Vector3((ix - 2) * .125f, (iy - 2) * .125f, (iz - 2) * .125f) + (m_pObject1->m_objectPos + m_pObject2->m_objectPos) * .5f;
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
	// �I�u�W�F�N�g�̕`��
	m_pObject2->Render(context, Color(hitObject ? Colors::Yellow : Colors::White));
	m_pObject1->Render(context, Color(hitObject ? Colors::Yellow : Colors::White));
	m_pObjectField->Render(context, Color(hitField ? Colors::Red : Colors::Gray));
	for (auto& obj : m_particles)
		obj->Render(context);
}

void MyGame::Finalize(GameContext & context)
{
	// �I�u�W�F�N�g�̉��
	m_pObject1->Finalize(context);
	m_pObject2->Finalize(context);
	for (auto& obj : m_particles)
		obj->Finalize(context);
}
