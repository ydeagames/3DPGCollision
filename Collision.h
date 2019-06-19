#pragma once

class CollisionObject;

class Collision
{
public:
	static bool IsHit(const CollisionObject& obj1, const CollisionObject& obj2);
};

