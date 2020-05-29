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

void MyGame::Initialize(GameContext & ctx)
{
	// �I�u�W�F�N�g�̏���
	auto object = std::make_unique<CollisionObject>();
	object->Initialize(ctx);
	object->m_objectPos = Vector3::Zero;
	object->m_objectColor = Colors::White;
	object->m_objectVel = Vector3::Zero;
	object->m_objectSize = Vector3::One;
	object->m_objectWeight = 1;
	m_objects.emplace_back(std::move(object));

	auto device = ctx.GetDR().GetD3DDevice();
	auto context = ctx.GetDR().GetD3DDeviceContext();

	// �e�N�X�`��
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Resources/bricks/bricks_Diffuse.png", nullptr, m_textureDiffuse.GetAddressOf()));
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Resources/bricks/bricks_Normal.png", nullptr, m_textureNormal.GetAddressOf()));
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Resources/bricks/bricks_Roughness.png", nullptr, m_textureRoughness.GetAddressOf()));

	// �G�t�F�N�g�̐���
	m_basicEffect = std::make_unique<DirectX::PBREffect>(device);
	// ���_�J���[(�L��)
	// m_basicEffect->SetTextureEnabled(true);
	// �v���~�e�B�u�I�u�W�F�N�g����
	std::vector<GeometricPrimitive::VertexType> vertices = {
		{ Vector3(-0.5f, +0.5f, 0.0f), Vector3::Forward, Vector2(0.0f, 0.0f) },
		{ Vector3(+0.5f, +0.5f, 0.0f), Vector3::Forward, Vector2(1.0f, 0.0f) },
		{ Vector3(+0.5f, -0.5f, 0.0f), Vector3::Forward, Vector2(1.0f, 1.0f) },
		{ Vector3(-0.5f, -0.5f, 0.0f), Vector3::Forward, Vector2(0.0f, 1.0f) },
	};
	std::vector<uint16_t> indices = {
		0, 1, 2, 0, 2, 3,
	};
	m_plane = GeometricPrimitive::CreateCustom(context, vertices, indices);
	m_plane->CreateInputLayout(m_basicEffect.get(), m_pInputLayout.GetAddressOf());
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

	if (Input::GetMouseButton(Input::Buttons::MouseRight))
	{
		auto mouse = Input::GetMousePosition();
		auto delta = lastMouse - mouse;
		yaw -= delta.x * .01f;
		pitch += delta.y * .01f;
		lastMouse = mouse;
	}
}

void MyGame::Render(GameContext & context)
{
	// �I�u�W�F�N�g�̕`��
	for (auto& obj : m_objects)
		obj->Render(context);

	m_basicEffect->SetWorld(Matrix::CreateScale(2) * Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.f));
	m_basicEffect->SetView(context.GetCamera().view);
	m_basicEffect->SetProjection(context.GetCamera().projection);

	m_basicEffect->SetLightDiffuseColor(0, Colors::White);
	m_basicEffect->SetLightDirection(0, Vector3::One);
	m_basicEffect->SetSurfaceTextures(m_textureDiffuse.Get(), m_textureNormal.Get(), m_textureRoughness.Get());
	m_plane->Draw(m_basicEffect.get(), m_pInputLayout.Get());
}

void MyGame::Finalize(GameContext & context)
{
	// �I�u�W�F�N�g�̉��
	for (auto& obj : m_objects)
		obj->Finalize(context);
}
