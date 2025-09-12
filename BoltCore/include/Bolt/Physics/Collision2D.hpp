#pragma once
#include "Bolt/Scene/EntityHandle.hpp"

namespace Bolt {
	struct Collision2D {
		EntityHandle entityA;
		EntityHandle entityB;
	};
}