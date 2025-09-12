#pragma once
#include "Bolt/Components/Collider2D.hpp"
#include "Bolt/Components/Rigidbody2D.hpp"
#include "Bolt/Physics/PhysicsSystem.hpp"
#include "Bolt/Graphics/Gizmos.hpp"
#include <box2d/box2d.h>

namespace Bolt {
	class BoxCollider2D : public Collider2D {
	public:
		BoxCollider2D() = default;

		void SetScale(const Vec2& scale, const Scene& scene);
		Vec2 GetScale();
		Vec2 GetLocalScale(const Scene& scene);

		// Note: has to be called when the transforms scale has been changed.
		void UpdateScale(const Scene& scene);

		void SetCenter(const Vec2& center, const Scene& scene);
		Vec2 GetCenter();
	private:
		friend class PhysicsSystem;
	};
}