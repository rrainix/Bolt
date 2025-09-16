 
#include "Bolt/btpch.hpp"
#include "Bolt/Components/Transform2D.hpp"

namespace Bolt {

	Transform2D Transform2D::FromPosition(const Vec2& pos) {
		Transform2D tr;
		tr.Position = pos;
		return tr;
	}
	Transform2D Transform2D::FromScale(const Vec2& scale) {
		Transform2D tr;
		tr.Scale = scale;
		return tr;
	}


	float Transform2D::GetRotationDegrees() const { return Degrees(Rotation); }
	float* Transform2D::GetModelMatrix(float out[16]) const {
		const float sx = (Scale.x == 0.0f) ? 1e-6f : Scale.x;
		const float sy = (Scale.y == 0.0f) ? 1e-6f : Scale.y;

		bx::mtxSRT(
			out,
			sx, sy, 1.0f,         
			0.0f, 0.0f, Rotation, 
			Position.x, Position.y, 0.0f
		);
		return out;
	}

	b2Rot Transform2D::GetB2Rotation() const {
		return  b2Rot(Cos(Rotation), Sin(Rotation));
	}
}