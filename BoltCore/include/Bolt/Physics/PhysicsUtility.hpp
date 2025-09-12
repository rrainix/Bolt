#pragma once

namespace Bolt {
	class Collider2D;
	class Rigidbody2D;
}

struct b2BodyId;
struct b2ShapeId;

namespace Bolt {
	class PhysicsUtility {
	public:
		static EntityHandle GetNativeEntityFromCollider(const Collider2D& collider);
		static EntityHandle GetNativeEntityFromRigidbody(const Rigidbody2D& rb);
		static EntityHandle GetNativeEntityFromBodyId(b2BodyId bodyId);
		static EntityHandle GetNativeEntityFromShapeId(b2ShapeId shapeId);
	};
}