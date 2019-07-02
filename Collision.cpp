#include "pch.h"
#include "Collision.h"
#include "CollisionObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

bool Collision::IsHit(const CollisionObject & obj1, const CollisionObject & obj2)
{
	//return obj1.m_objectCollider.Intersects(obj2.m_objectCollider);
	auto& p1 = obj1.m_objectCollider.Center;
	auto& p2 = obj2.m_objectCollider.Center;
	auto& h1 = obj1.m_objectCollider.Radius;
	auto& h2 = obj2.m_objectCollider.Radius;
	return Vector3::DistanceSquared(p1, p2) < std::pow(h1 + h2, 2);
}
