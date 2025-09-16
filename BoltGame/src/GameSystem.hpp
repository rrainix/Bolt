#pragma once
#include "bolt/Bolt.hpp"
#include <optional>

namespace Bolt {
	class GameSystem : public ISystem {

	public:
		void Awake(Scene& scene)override;
		void Start(Scene& scene)override;
		void Update(Scene& scene) override;
		void OnDestroy(Scene& scene) override;

	private:
		void CreateCamera(Scene& scene);
		void CreateEntity(Scene& scene, Vec2 position);

		TextureHandle BlockTexture;
		TextureHandle SquareTexture;
		TextureHandle TorchTexture;
	};
}