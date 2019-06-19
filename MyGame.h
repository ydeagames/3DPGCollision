#pragma once
#include "CollisionObject.h"

class GameContext;

class MyGame
{
private:
	// �I�u�W�F�N�g�֘A
	std::unique_ptr<CollisionObject>	m_pObject1;			// �I�u�W�F�N�g�P
	std::unique_ptr<CollisionObject>	m_pObject2;			// �I�u�W�F�N�g�Q

public:
	// ����
	void Initialize(GameContext& context);
	// �X�V
	void Update(GameContext& context);
	// �`��
	void Render(GameContext& context);
	// �j��
	void Finalize(GameContext& context);
};

