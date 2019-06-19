#include "pch.h"
#include "Collision.h"
#include "CollisionObject.h"

bool Collision::IsHit(const CollisionObject & obj1, const CollisionObject & obj2)
{
	//return obj1.m_objectCollider.Intersects(obj2.m_objectCollider);
	auto& p1 = obj1.m_objectCollider.Center;
	auto& p2 = obj2.m_objectCollider.Center;
	auto& h1 = obj1.m_objectCollider.Extents;
	auto& h2 = obj2.m_objectCollider.Extents;
	return (std::abs(p2.x - p1.x) < h1.x + h2.x)
		&& (std::abs(p2.y - p1.y) < h1.y + h2.y)
		&& (std::abs(p2.z - p1.z) < h1.z + h2.z);
}
