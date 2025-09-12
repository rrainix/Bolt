#pragma once
#include "Bolt/Collections/Vec2.hpp"
#include "Bolt/Utils/Random.hpp"

namespace Bolt {
	inline float Dot(const Vec2& a, const Vec2& b) noexcept {
		return glm::dot(a, b);
	}

	inline float Length(const Vec2& a) noexcept {
		return glm::length(a);
	}

	inline float LengthSquared(const Vec2& a) noexcept {
		return Dot(a, a);
	}

	inline Vec2 Normalized(const Vec2& a) noexcept {
		float len = Length(a);
		return (len > 0.0f) ? (a * (1.0f / len)) : Vec2(0.0f, 0.0f);
	}

	// Distanz
	inline float Distance(const Vec2& a, const Vec2& b) noexcept {
		return Length(a - b);
	}

	inline float DistanceSquared(const Vec2& a, const Vec2& b) noexcept {
		return LengthSquared(a - b);
	}

	// Rotation (um Ursprung)
	inline Vec2 Rotated(const Vec2& v, float radians) noexcept {
		float c = Bolt::Cos(radians);
		float s = Bolt::Sin(radians);
		return Vec2(v.x * c - v.y * s, v.x * s + v.y * c);
	}

	inline Vec2 Lerp(const Vec2& a, const Vec2& b, float t) noexcept {
		return a + (b - a) * t;
	}

	// Winkel
	inline float Angle(const Vec2& a) noexcept {
		return std::atan2(a.y, a.x);
	}


	inline Vec2 Zero() noexcept { return Vec2(0.0f, 0.0f); }
	inline Vec2 One() noexcept { return Vec2(1.0f, 1.0f); }
	inline Vec2 Up() noexcept { return Vec2(0.0f, 1.0f); }
	inline Vec2 Down() noexcept { return Vec2(0.0f, -1.0f); }
	inline Vec2 Left() noexcept { return Vec2(-1.0f, 0.0f); }
	inline Vec2 Right() noexcept { return Vec2(1.0f, 0.0f); }


	inline Vec2 FromAngle(float radians) noexcept {
		return Vec2(std::cos(radians), std::sin(radians));
	}

	inline Vec2 RandomInCircle(float radius, Random& rand) noexcept {
		const float twoPi = 6.28318530717958647692f;
		float theta = rand.Next(0.0f, twoPi);
		float u = rand.Next(0.0f, 1.0f);
		float r = radius * std::sqrt(u);          // gleichm��ige Fl�chenverteilung!
		return FromAngle(theta) * r;
	}
	inline Vec2 RandomOnCircle(float radius, Random& rand) noexcept {
		const float twoPi = 6.28318530717958647692f;
		float theta = rand.Next(0.0f, twoPi);
		return FromAngle(theta) * radius;
	}
}