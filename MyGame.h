#pragma once
#include "CollisionObject.h"

class GameContext;

class MyGame
{
private:
	// �I�u�W�F�N�g�֘A
	std::vector<std::unique_ptr<CollisionObject>> m_objects;	// �I�u�W�F�N�g

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

