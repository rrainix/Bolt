#pragma once
#include "bolt/Bolt.hpp"
#include <optional>

namespace Bolt {
	class GameSystem : public ISystem {

	public:
		void Start(Scene& scene)override;
		void Update(Scene& scene) override;
		void OnDestroy(Scene& scene) override;
	};
}