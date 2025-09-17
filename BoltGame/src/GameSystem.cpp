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
		Entity entity = CreatePhysicsEntity(scene, Vec2(0, -1), BodyType::Static);
		entity.GetComponent<Transform2D>().Scale.x = 1000;
		entity.GetComponent<BoxCollider2D>().UpdateScale(scene);
		CreateCamera(scene);
	}

	void GameSystem::OnDestroy(Scene& scene) {
		Logger::Message("Destroying System");
	}

	void GameSystem::Update(Scene& scene) {
		Vec2 mousePosition = Camera2D().Main()->ScreenToWorld(Input::MousePosition());

		Gizmos::SetColor(Color::Green());

		for (auto [ent, transform, boxCollider2D] : scene.GetRegistry().view<Transform2D, BoxCollider2D>().each()) {
			Gizmos::DrawBox(boxCollider2D.GetBodyPosition(), boxCollider2D.GetScale(), boxCollider2D.GetRotationDegrees());
		}

		Gizmos::SetColor(Color::Gray());

		for (auto [ent, transform] : scene.GetRegistry().view<Transform2D>().each()) {
			Gizmos::DrawBox(transform.Position, transform.Scale, transform.GetRotationDegrees());
		}

		if (Input::GetKeyDown(KeyCode::E)) {
			CreatePhysicsEntity(scene, mousePosition);
		}
		if (Input::GetKeyDown(KeyCode::P)) {
			CreateParticleSystem(scene, mousePosition);
		}
		if (Input::GetKeyDown(KeyCode::F3)) Gizmos::SetEnabled(!Gizmos::IsEnabled());

		if (Input::GetKeyDown(KeyCode::R)) {
			SceneManager::ReloadScene(scene.GetName());
		}

		CameraController(scene);
	}

	void GameSystem::CreateCamera(Scene& scene) {
		Entity entity = scene.CreateEntity();
		entity.AddComponent<Camera2D>();
	}

	void GameSystem::CameraController(Scene& scene) {
		Camera2D& mainCam = *Camera2D().Main();
		Vec2 mousePosition = mainCam.ScreenToWorld(Input::MousePosition());

		if (Input::GetMouseDown(1))
		{
			startCamMousePosition = mousePosition;
		}
		if (Input::GetMouse(1))
		{
			float controlSpeed = 2;
			mainCam.AddPosition((startCamMousePosition - mousePosition) * (Time::GetDeltaTimeUnscaled() * 50 * controlSpeed));
		}

		float zoomSpeed = 1;
		mainCam.AddOrthographicSize(-Input::ScrollValue() * Time::GetDeltaTimeUnscaled() * 10 * zoomSpeed);
	}

	void GameSystem::CreateEntity(Scene& scene, Vec2 position) {
		Entity entity = scene.CreateEntity();
		entity.GetComponent<Transform2D>().Position = position;
		SpriteRenderer& spriteRenderer = entity.AddComponent<SpriteRenderer>();
		spriteRenderer.Sprite = BlockTexture;
	}
	Entity GameSystem::CreatePhysicsEntity(Scene& scene, Vec2 position, BodyType body) {
		Entity entity = scene.CreateEntity();
		entity.GetComponent<Transform2D>().Position = position;
		SpriteRenderer& spriteRenderer = entity.AddComponent<SpriteRenderer>();
		spriteRenderer.Sprite = BlockTexture;
		BoxCollider2D& boxCollider = entity.AddComponent<BoxCollider2D>();
		Rigidbody2D& rb2D = entity.AddComponent<Rigidbody2D>();
		rb2D.SetBodyType(body);
		return entity;
	}
	void GameSystem::CreateParticleSystem(Scene& scene, Vec2 position) {
		Entity entity = scene.CreateEntity();
		entity.GetComponent<Transform2D>().Position = position;
		ParticleSystem2D& ptSystem = entity.AddComponent<ParticleSystem2D>();
		ptSystem.Play();
	}
}