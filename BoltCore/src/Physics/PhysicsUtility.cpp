#include "Bolt/btpch.hpp"
#include "Bolt/Physics/PhysicsUtility.hpp"
#include "Bolt/Components/Rigidbody2D.hpp"
#include "Bolt/Components/Collider2D.hpp"

namespace Bolt {
	EntityHandle PhysicsUtility::GetNativeEntityFromCollider(const Collider2D& collider) {
		void* ud = b2Body_GetUserData(collider.m_BodyId);
		return static_cast<EntityHandle>(reinterpret_cast<uintptr_t>(ud));
	}
	EntityHandle PhysicsUtility::GetNativeEntityFromRigidbody(const Rigidbody2D& rb) {
		void* ud = b2Body_GetUserData(rb.GetBodyHandle());
		return static_cast<EntityHandle>(reinterpret_cast<uintptr_t>(ud));
	}
	EntityHandle PhysicsUtility::GetNativeEntityFromBodyId(b2BodyId bodyId) {
		void* ud = b2Body_GetUserData(bodyId);
		return static_cast<EntityHandle>(reinterpret_cast<uintptr_t>(ud));
	}
	EntityHandle PhysicsUtility::GetNativeEntityFromShapeId(b2ShapeId shapeId) {
		void* ud = b2Body_GetUserData(b2Shape_GetBody(shapeId));
		return static_cast<EntityHandle>(reinterpret_cast<uintptr_t>(ud));
	}
} 