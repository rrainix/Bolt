#pragma once
#include "Bolt/Graphics/TextureHandle.hpp"

namespace Bolt {
	struct SpriteRenderer {
		SpriteRenderer() = default;

		short SortingOrder{0};
		uint8_t SortingLayer{0};
		TextureHandle Sprite;
		Color Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}