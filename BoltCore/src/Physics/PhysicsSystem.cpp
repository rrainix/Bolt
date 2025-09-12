#include "Bolt/btpch.hpp"
#include "Bolt/Physics/PhysicsSystem.hpp"
#include "Bolt/Components/Rigidbody2D.hpp"
#include "Bolt/Physics/Box2DWorld.hpp"
#include "Bolt/Scene/SceneManager.hpp"
#include "Bolt/Scene/Scene.hpp"
#include "Bolt/Debug/Logger.hpp"


namespace Bolt {
	bool PhysicsSystem::s_IsEnabled = true;
	Box2DWorld PhysicsSystem::s_MainWorld{};

	void PhysicsSystem::FixedUpdate(float dt) {
		if (!s_IsEnabled)return;

		//for (auto& scene : SceneManager::s_LoadedScenes)
		//{
		//	for (auto [ent, rb, tf] : scene->GetRegistry().view<Rigidbody2D, Transform2D>().each()) {
		//		rb.SetTransform(tf);
		//	}
		//	for (auto [ent, boxCollider, tf] : scene->GetRegistry().view<BoxCollider2D, Transform2D>().each()) {
		//		boxCollider.SetTransform(tf);
		//	}
		//}

		s_MainWorld.Step(dt);
		s_MainWorld.GetDispatcher().process(s_MainWorld.GetWorldID());

		for (auto& scene : SceneManager::s_LoadedScenes)
		{
			for (auto [ent, rb, tf] : scene->GetRegistry().view<Rigidbody2D, Transform2D>().each()) {
				tf.Position = rb.GetPosition();
				tf.Rotation = rb.GetRotation();
			}
		}
	}
}