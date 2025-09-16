#include "GameSystem.hpp"
#include "Bolt/Physics/Collision2D.hpp"
#include "Bolt/Audio/AudioManager.hpp"
#include "Bolt/Utils/Timer.hpp"
#include "Bolt/Components/ParticleSystem2D.hpp"
#include "Bolt/Utils/Random.hpp"
#include <variant>

namespace Bolt {

	void GameSystem::Awake(Scene& scene) {
		BlockTexture = TextureManager::LoadTexture("Assets/Textures/block.png");
	}

	void GameSystem::Start(Scene& scene) {
		CreateCamera(scene);
	}

	void GameSystem::OnDestroy(Scene& scene) {
		Logger::Message("Destroying System");
	}

	void GameSystem::Update(Scene& scene) {
		Vec2 mousePosition = Camera2D().Main()->ScreenToWorld(Input::MousePosition());

		if (Input::GetKeyDown(KeyCode::E)) {
			CreateEntity(scene, mousePosition);
		}
		if (Input::GetKeyDown(KeyCode::R)) {
			SceneManager::ReloadScene(scene.GetName());
		}
	}

	void GameSystem::CreateCamera(Scene& scene) {
		Entity entity = scene.CreateEntity();
		entity.AddComponent<Camera2D>();
	}

	void GameSystem::CreateEntity(Scene& scene, Vec2 position) {
		Entity entity = scene.CreateEntity();
		entity.GetComponent<Transform2D>().Position = position;
		SpriteRenderer& spriteRenderer = entity.AddComponent<SpriteRenderer>();
		spriteRenderer.Sprite = BlockTexture;
	}
}